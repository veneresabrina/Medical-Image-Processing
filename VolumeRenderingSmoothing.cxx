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
#include <vtkInteractorStyleImage.h>
#include <vtkImageGaussianSmooth.h>

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

   //Il filtro di smoothing rende il volume più liscio
   vtkSmartPointer<vtkImageGaussianSmooth> gaussianSmoothFilter = 
    vtkSmartPointer<vtkImageGaussianSmooth>::New();
  gaussianSmoothFilter->SetInputConnection(reader->GetOutputPort());
  gaussianSmoothFilter->Update();

   //Il volume è realizzato tramite un mapper ray-casting
  vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper =
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
  volumeMapper->SetInputConnection(gaussianSmoothFilter->GetOutputPort());

  // La funzione di trasferimento di opacità mostra tessuti diversi con opacità diverse
  // In questo caso distingue il tessuto epatico dallo sfondo
  vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity =
    vtkSmartPointer<vtkPiecewiseFunction>::New();
  volumeScalarOpacity->AddPoint(0,    0.00);
  volumeScalarOpacity->AddPoint(500,  0.15);
  volumeScalarOpacity->AddPoint(1000, 0.15);
  volumeScalarOpacity->AddPoint(1150, 0.85);

  // L'opacità derivata dalla funzione precedente caratterizza le proprietà del volume
  // L'interpolazione è di tipo lineare
  // L'opzione ShadeOn dà direzionalità alla luce (accentua l'effetto 3D) il quale è modulato dai coefficienti Ambient, Diffuse e Specular
  vtkSmartPointer<vtkVolumeProperty> volumeProperty =
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->SetScalarOpacity(volumeScalarOpacity);
  volumeProperty->SetInterpolationTypeToLinear();
  volumeProperty->ShadeOn();
  volumeProperty->SetAmbient(0.9);
  volumeProperty->SetDiffuse(0.6);
  volumeProperty->SetSpecular(0.2);

   // Si definisce l'attore
  vtkSmartPointer<vtkVolume> volume =
    vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  // Si aggiunge il volume al renderer
  ren->AddViewProp(volume);

  // La visuale iniziale del volume è settata 700 mm a sinistra del paziente.
  // Il punto focale è il centro del volume
  vtkCamera *camera = ren->GetActiveCamera();
  double *c = volume->GetCenter();
  camera->SetViewUp (0, 0, -1);
  camera->SetPosition (c[0], c[1] - 700, c[2]);
  camera->SetFocalPoint (c[0], c[1], c[2]);
  camera->Azimuth(30.0);
  camera->Elevation(30.0);

  // Definizione del colore di Background per il renderer
  ren->SetBackground(0.8,0.8,0.8);
  // Definizione della dimensione della Rendering Window
  renWin->SetSize(640, 480);

  // Inizio dell'interazione
  iren->Start();

  return EXIT_SUCCESS;
}