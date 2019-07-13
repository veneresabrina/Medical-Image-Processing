//CTRL Left Mouse slices through the image
//SHIFT Middle Mouse slices through the image
//CTRL Right Mouse spins the camera

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageProperty.h>
#include <vtkImageSlice.h>
#include <vtkInteractorStyleImage.h>
#include <vtkCamera.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    cout << "Usage: " << argv[0] << "myDirectory" << endl;
    return EXIT_FAILURE;
  }

    // Definizione viewport ranges
  //i = 1 sarà in alto a sinistra
  //i = 2 sarà in basso a sinistra
  //i = 3 sarà a destra
  double xmins[3] = {0,0,0.5};
  double xmaxs[3] = {0.5,0.5,1};
  double ymins[3] = {0.5,0,0};
  double ymaxs[3]= {1,0.5,1};


//Crea reader, RenderWindow and RenderWindowInteractor
  vtkSmartPointer<vtkDICOMImageReader> reader =
  vtkSmartPointer<vtkDICOMImageReader>::New();
  reader->SetDirectoryName(argv[1]);
  reader->Update();
vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);
     

  for(unsigned i = 0; i < 3; i++)
    {
  //Crea il Renderer
    vtkSmartPointer<vtkRenderer> ren =
    vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(ren);
	ren->SetViewport(xmins[i],ymins[i],xmaxs[i],ymaxs[i]);

  //Crea il mapper - ImageResliceMapper
    vtkSmartPointer<vtkImageResliceMapper> imMapper =
    vtkSmartPointer<vtkImageResliceMapper>::New();
  imMapper->SetInputConnection(reader->GetOutputPort());
  imMapper->SliceFacesCameraOn();
  imMapper->SliceAtFocalPointOn();
  imMapper->BorderOff();


  //Crea ImageProperty
    vtkSmartPointer<vtkImageProperty> improp =
  vtkSmartPointer<vtkImageProperty>::New();
	improp->SetColorWindow(2000);
	improp->SetColorLevel(800);
	improp->SetAmbient(0.6);
	improp->SetDiffuse(0.4);
	improp->SetOpacity(1.0);
	improp->SetInterpolationTypeToLinear();

	//Crea l'attore - ImageSlice
	vtkSmartPointer<vtkImageSlice> im =
  vtkSmartPointer<vtkImageSlice>::New();
	im->SetMapper(imMapper);
	im->SetProperty(improp);


	//Aggiungi l'attore al renderer
	ren->AddActor(im);
	ren->SetBackground(0.1,0.2,0.4);
	renWin->SetSize(1024,768);

  //Crea Style (definisce l'orientazione degli oggetti nella finestra)
    vtkSmartPointer<vtkInteractorStyleImage> style =
  vtkSmartPointer<vtkInteractorStyleImage>::New();
	style->SetInteractionModeToImageSlicing();

  // x is right, y is up
  style->SetZViewRightVector(1, 0, 0);
  style->SetZViewUpVector(0, 1, 0);

  // x is right, z is up
  style->SetYViewRightVector(1, 0, 0);
  style->SetYViewUpVector(0, 0, -1);

  // z is left, y is up
  style->SetXViewRightVector(0, -1, 0);
  style->SetXViewUpVector(0, 0, -1);

  //Crea il testo (aggiunge un attore alla finestra di rendering)
  style->GetCurrentImageProperty();
  vtkSmartPointer<vtkTextActor> textActor = 
    vtkSmartPointer<vtkTextActor>::New();

  textActor->GetTextProperty()->SetFontSize ( 24 );
  ren->AddActor2D (textActor);

  //L'interattore è definito da Style
	iren->SetInteractorStyle(style);
	renWin->SetInteractor(iren);

  //Rendering dell'immagine
	renWin->Render();
  vtkCamera *camera = ren->GetActiveCamera();
  camera->ParallelProjectionOn();
  camera->Zoom(2);
  ren->ResetCameraClippingRange();


  style->SetCurrentRenderer(ren);

    if (i == 0)
      {
      style->SetImageOrientation(
        style->GetXViewRightVector(),
        style->GetXViewUpVector());
	    textActor->SetInput ("sagittale");
      }
    else if (i == 1)
      {
      style->SetImageOrientation(
		style->GetYViewRightVector(),
        style->GetYViewUpVector());
	    textActor->SetInput ("coronale");
      }
    else if (i == 2)
      {
      style->SetImageOrientation(
        style->GetZViewRightVector(),
        style->GetZViewUpVector());        
	    textActor->SetInput ("assiale");
      }


  renWin->Render();

   }


  iren->Start();

  return EXIT_SUCCESS;
}