#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
#include <QImage>
#include <QList>
#include <QMainWindow>
#include <QPieSlice>
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void loadImage();
  void onSliceClicked(QPieSlice *slice);

private:
  // Una tripletta RGB con il suo numero di occorrenze nell'immagine.
  struct Record {
    QRgb color;
    int count;
  };

  Ui::MainWindow *ui;

  QImage _image;
  QString _imageName;
  QString _lastDir;

  QPieSlice *_selectedSlice = nullptr;

  // Triplette in ordine di lettura dei pixel, piu' una hash
  // colore -> indice del record nella lista, che fa da lookup per
  // unicita' e conteggio in tempo costante.
  QList<Record> _triplets;
  QHash<QRgb, int> _lookup;

  // Conteggi per intensita' di ogni canale.
  // L'indice della lista e' l'intensita' (0-255), il valore e' il conteggio.
  QList<int> _channelR;
  QList<int> _channelG;
  QList<int> _channelB;

  void setupCharts();
  void analyzeImage();
  void displayImage();
  void updateTripletChart();
  void updateRgbChart();

  static const QString TRIPLET_TITLE;
};

#endif // MAINWINDOW_H