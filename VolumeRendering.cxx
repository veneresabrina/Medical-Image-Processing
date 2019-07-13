#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>


int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    cout << "Usage: " << argv[0] << "myDirectory" << endl;
    return EXIT_FAILURE;
  }

  //Crea il renderer, la render window e l'interactor.
  vtkSmartPointer<vtkRenderer> ren =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren);
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  //Legge le immagini in formato DICOM che si trovano nella cartella indicata nel prompt dei comandi

  vtkSmartPointer<vtkDICOMImageReader> reader =
  vtkSmartPointer<vtkDICOMImageReader>::New();
  reader->SetDirectoryName(argv[1]);
  reader->Update();
  
  //Il volume è realizzato tramite un mapper ray-casting
  vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
  volumeMapper->SetInputConnection(reader->GetOutputPort());

  //La funzione di trasferimento di colore mappa i voxel in colori RGB 
  //Si cerca di rendere un colore per i tessuti molli e un altro per le ossa
  vtkSmartPointer<vtkColorTransferFunction>volumeColor =
    vtkSmartPointer<vtkColorTransferFunction>::New();
  volumeColor->AddRGBPoint(0,    0.0, 0.0, 0.0);
  volumeColor->AddRGBPoint(500,  1.0, 0.5, 0.3);
  volumeColor->AddRGBPoint(1000, 1.0, 0.6, 0.4);
  volumeColor->AddRGBPoint(1150, 1.0, 0.7, 0.5);

  // La funzione di trasferimento di opacità mostra tessuti diversi con opacità diverse
  vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity =
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  volumeScalarOpacity->AddPoint(0,    0.00);
  volumeScalarOpacity->AddPoint(500,  0.45);
  volumeScalarOpacity->AddPoint(1000, 0.45);
  volumeScalarOpacity->AddPoint(1150, 0.25);

  // La funzione gradiente di opacità diminuisce l'opacità nelle regioni piatte del volume accentuando i confini delle regioni
  vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  volumeGradientOpacity->AddPoint(0,   0.0);
  volumeGradientOpacity->AddPoint(90,  0.5);
  volumeGradientOpacity->AddPoint(100, 1.0);

  // Il colore e l'opacità derivati dalle funzioni precedenti sono assegnate alle proprietà del volume
  // L'interpolazione è di tipo lineare
  // L'opzione ShadeOn dà direzionalità alla luce (accentua l'effetto 3D) il quale è modulato dai coefficienti Ambient, Diffuse e Specular
  vtkSmartPointer<vtkVolumeProperty> volumeProperty =
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->SetColor(volumeColor);
  volumeProperty->SetScalarOpacity(volumeScalarOpacity);
  volumeProperty->SetGradientOpacity(volumeGradientOpacity);
  volumeProperty->SetInterpolationTypeToLinear();
  volumeProperty->ShadeOn();
  volumeProperty->SetAmbient(0.4);
  volumeProperty->SetDiffuse(0.6);
  volumeProperty->SetSpecular(0.4);

  // Si definisce l'attore
  vtkSmartPointer<vtkVolume> volume =
    vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  // Si aggiunge il volume al renderer
  ren->AddViewProp(volume);

  // La visuale iniziale del volume è settata 400 mm a sinistra del paziente.
  // Il punto focale è il centro del volume e la visuale è spostata 400 mm più lontano rispetto ad esso.
  vtkCamera *camera = ren->GetActiveCamera();
  double *c = volume->GetCenter();
  camera->SetViewUp (0, 0, -1);
  camera->SetPosition (c[0] - 400, c[1] -400, c[2]);
  camera->SetFocalPoint (c[0], c[1], c[2]);
  camera->Azimuth(90.0); 
  camera->Elevation(0.0);

  // Definizione del colore di Background per il renderer
    ren->SetBackground(0.8,0.8,0.8);

  // Definizione della dimensione della Rendering Window
  renWin->SetSize(640, 480);

  // Inizio dell'interazione
  iren->Start();

  return EXIT_SUCCESS;
}