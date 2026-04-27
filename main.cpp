#include <vtkNew.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkButtonWidget.h>
#include <vtkTexturedButtonRepresentation2D.h> // For the button look
#include <vtkSphereSource.h>
#include <vtkCoordinate.h>

// This is the "Action" that happens when you click Play
class vtkButtonCallback : public vtkCommand {
public:
    static vtkButtonCallback* New() { return new vtkButtonCallback; }
    void Execute(vtkObject* caller, unsigned long, void*) override {
        printf("Play Button Clicked! Starting Simulation...\n");
        // Here is where you will trigger Step 2/3 later!
    }
};

int main() {
    vtkNew<vtkOpenVRRenderer> renderer;
    vtkNew<vtkOpenVRRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkOpenVRRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    // 1. THE FLOOR (Step 1)
    vtkNew<vtkPlaneSource> plane;
    plane->SetNormal(0, 0, 1);
    vtkNew<vtkPolyDataMapper> floorMapper;
    floorMapper->SetInputConnection(plane->GetOutputPort());
    vtkNew<vtkActor> floorActor;
    floorActor->SetMapper(floorMapper);
    floorActor->GetProperty()->SetColor(0.2, 0.5, 0.2);
    renderer->AddActor(floorActor);

    // 2. THE PLAY BUTTON (The Lecturer's Requirement)
    // Create a sphere to act as the button
    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(0.1);

    vtkNew<vtkTexturedButtonRepresentation2D> buttonRep;
    buttonRep->SetNumberOfStates(1);
    // Position the button in front of the user (x, y, z)
    buttonRep->SetPlaceFactor(1);

    vtkNew<vtkButtonWidget> buttonWidget;
    buttonWidget->SetInteractor(interactor);
    buttonWidget->SetRepresentation(buttonRep);

    // Link the "Click" to our Callback function above
    vtkNew<vtkButtonCallback> callback;
    buttonWidget->AddObserver(vtkCommand::StateChangedEvent, callback);

    renderWindow->Initialize();

    // Position the button in the 3D space
    double bounds[6] = { -0.1, 0.1, 1.0, 1.2, 1.0, 1.2 }; // Putting it roughly at chest height
    buttonRep->PlaceWidget(bounds);

    buttonWidget->On(); // Turn the button on
    interactor->Start();

    return 0;
}