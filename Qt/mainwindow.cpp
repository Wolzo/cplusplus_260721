#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QChart>
#include <QChartView>
#include <QColor>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QLineSeries>
#include <QPainter>
#include <QPieSeries>
#include <QPieSlice>
#include <QPixmap>
#include <QPushButton>
#include <QValueAxis>

#include <utility>

const QString MainWindow::TRIPLET_TITLE = "Composizione triplette RGB";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setupCharts();

  connect(ui->loadImgBtn, &QPushButton::clicked, this, &MainWindow::loadImage);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupCharts() {
  QChart *tripletChart = new QChart();
  tripletChart->setTitle(TRIPLET_TITLE);
  tripletChart->legend()->hide();
  ui->tripletsChart->setChart(tripletChart);
  ui->tripletsChart->setRenderHint(QPainter::Antialiasing);

  QChart *rgbChart = new QChart();
  rgbChart->setTitle("Intensita' per canale");
  ui->rgbChart->setChart(rgbChart);
  ui->rgbChart->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  displayImage();
}

void MainWindow::loadImage() {
  const QString path = QFileDialog::getOpenFileName(
      this, "Seleziona immagine",
      _lastDir.isEmpty() ? QCoreApplication::applicationDirPath() : _lastDir,
      "Immagini (*.png *.jpg *.jpeg *.bmp)");

  if (path.isEmpty())
    return;

  const QFileInfo info(path);
  _imageName = info.fileName();
  _lastDir = info.absolutePath();  // memorizza la cartella per la prossima apertura

  QImageReader reader(path);
  reader.setAutoTransform(true);

  const QImage loaded = reader.read();

  if (loaded.isNull()) {
    ui->imgLabel->setText("Errore: " + reader.errorString());
    return;
  }

  _image = loaded;

  analyzeImage();
  displayImage();
  updateTripletChart();
  updateRgbChart();
}

void MainWindow::analyzeImage() {
  if (_image.isNull())
    return;

  // Azzeramento: ogni immagine parte da strutture pulite.
  _selectedSlice = nullptr;
  _triplets.clear();
  _lookup.clear();
  _channelR = QList<int>(256, 0);
  _channelG = QList<int>(256, 0);
  _channelB = QList<int>(256, 0);

  // reset del titolo
  ui->tripletsChart->chart()->setTitle(TRIPLET_TITLE);

  // Scansione riga per riga (dall'alto a sinistra).
  for (int y = 0; y < _image.height(); ++y) {
    for (int x = 0; x < _image.width(); ++x) {
      const QRgb p = _image.pixel(x, y);
      const int r = qRed(p);
      const int g = qGreen(p);
      const int b = qBlue(p);

      // popolo le intensita' di ogni canale
      _channelR[r]++;
      _channelG[g]++;
      _channelB[b]++;

      // Se il colore esiste gia' incremento il conteggio
      // del suo record, altrimenti aggiungo un nuovo record e registro l'indice.
      const QRgb key = qRgb(r, g, b);
      const int index = _lookup.value(key, -1);
      if (index != -1) {
        _triplets[index].count++;
      } else {
        Record record;
        record.color = key;
        record.count = 1;
        _triplets.append(record);
        _lookup.insert(key, _triplets.size() - 1);
      }
    }
  }
}

void MainWindow::displayImage() {
  if (_image.isNull())
    return;

  const QPixmap pixmap = QPixmap::fromImage(_image);
  const QPixmap scaled = pixmap.scaled(ui->imgLabel->size(), Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);

  ui->imgLabel->setPixmap(scaled);
  ui->pathLabel->setText(_imageName);
}

/*
 * Grafico a torta con una fetta per tripletta, colorata col colore reale della
 * tripletta e con ampiezza proporzionale al numero di occorrenze normalizzato.
 *
 * Si usa la torta perche' e' l'unico grafico di Qt Charts che permette di
 * colorare ogni elemento singolarmente, mentre nel bar chart il colore vale per l'intero set.
 * Le fette sono aggiunte nell'ordine di lettura di _triplets.
 */
void MainWindow::updateTripletChart() {
  const int totalPixels = _image.width() * _image.height();
  if (totalPixels == 0 || _triplets.isEmpty())
    return;

  QPieSeries *series = new QPieSeries();

  for (const Record &rec : std::as_const(_triplets)) {
    const double freq = static_cast<double>(rec.count) / totalPixels;
    QPieSlice *slice = series->append("", freq);
    slice->setColor(QColor(rec.color));
    slice->setBorderColor(QColor(rec.color));
  }

  series->setPieSize(0.95);
  series->setHoleSize(0.35);

  connect(series, &QPieSeries::clicked, this, &MainWindow::onSliceClicked);

  QChart *chart = ui->tripletsChart->chart();
  chart->removeAllSeries();
  chart->addSeries(series);
}

/*
 * Istogramma delle intensita' per canale, tre spezzate (R, G, B).
 * Ogni serie ha 256 punti (intensita' -> frequenza normalizzata)
 */
void MainWindow::updateRgbChart() {
  const int totalPixels = _image.width() * _image.height();
  if (totalPixels == 0)
    return;

  QLineSeries *seriesR = new QLineSeries();
  QLineSeries *seriesG = new QLineSeries();
  QLineSeries *seriesB = new QLineSeries();

  seriesR->setName("R");
  seriesG->setName("G");
  seriesB->setName("B");

  seriesR->setColor(Qt::red);
  seriesG->setColor(Qt::green);
  seriesB->setColor(Qt::blue);

  double maxY = 0.0;
  for (int i = 0; i < 256; ++i) {
    const double yR = static_cast<double>(_channelR[i]) / totalPixels;
    const double yG = static_cast<double>(_channelG[i]) / totalPixels;
    const double yB = static_cast<double>(_channelB[i]) / totalPixels;

    seriesR->append(i, yR);
    seriesG->append(i, yG);
    seriesB->append(i, yB);

    maxY = qMax(maxY, qMax(yR, qMax(yG, yB)));
  }

  QChart *chart = ui->rgbChart->chart();
  chart->removeAllSeries();

  const QList<QAbstractAxis *> oldAxes = chart->axes();
  for (QAbstractAxis *axis : oldAxes){
    chart->removeAxis(axis);
  }

  chart->addSeries(seriesR);
  chart->addSeries(seriesG);
  chart->addSeries(seriesB);

  QValueAxis *axisX = new QValueAxis();
  axisX->setRange(0, 255);
  axisX->setTitleText("Intensita'");
  axisX->setLabelFormat("%d");

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, maxY);
  axisY->setTitleText("Frequenza normalizzata");

  chart->addAxis(axisX, Qt::AlignBottom);
  chart->addAxis(axisY, Qt::AlignLeft);

  seriesR->attachAxis(axisX);
  seriesR->attachAxis(axisY);
  seriesG->attachAxis(axisX);
  seriesG->attachAxis(axisY);
  seriesB->attachAxis(axisX);
  seriesB->attachAxis(axisY);
}

/*
 * Quando si clicca una fetta, viene staccata (effetto exploded) e il
 * titolo del grafico ne mostra i valori RGB e la percentuale. Un secondo click
 * sulla stessa fetta la riattacca e ripristina il titolo. Solo una fetta e'
 * selezionata per volta. Si e' scelta questa soluzione perche' le label di
 * alcune fette venivano disegnate fuori dall'area visibile.
 */
void MainWindow::onSliceClicked(QPieSlice *slice) {
  if (_selectedSlice && _selectedSlice != slice)
    _selectedSlice->setExploded(false);

  const bool nowExploded = !slice->isExploded();
  slice->setExploded(nowExploded);

  QChart *chart = ui->tripletsChart->chart();

  if (nowExploded) {
    const QColor c = slice->color();
    const double perc = slice->percentage() * 100.0;
    chart->setTitle(TRIPLET_TITLE + " - " +
                    QString("RGB(%1, %2, %3)  -  %4%")
                        .arg(c.red())
                        .arg(c.green())
                        .arg(c.blue())
                        .arg(perc, 0, 'f', 2));
    _selectedSlice = slice;
  } else {
    chart->setTitle(TRIPLET_TITLE);
    _selectedSlice = nullptr;
  }
}