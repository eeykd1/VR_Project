#include <vtkNew.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

int main() {
    // --- STEP 1: INITIALIZE VR SYSTEM ---
    vtkNew<vtkOpenVRRenderer> renderer;
    vtkNew<vtkOpenVRRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkOpenVRRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);

    // We MUST initialize here so the headset "wakes up" 
    // and starts reporting its position to the PC.
    renderWindow->Initialize();

    // --- STEP 2: DYNAMIC POSITION CHECK ---
    // We ask the camera (the headset) where it is in the room right now.
    double headPos[3];
    renderer->GetActiveCamera()->GetPosition(headPos);

    // We save the X and Y (horizontal plane) but ignore Z (we want the floor at 0)
    double currentX = headPos[0];
    double currentY = headPos[1];

    // --- STEP 3: THE SOURCE & MAPPER ---
    vtkNew<vtkPlaneSource> planeSource;
    // Create a 10m x 10m blueprint centered at (0,0)
    planeSource->SetOrigin(-5.0, -5.0, 0.0);
    planeSource->SetPoint1(5.0, -5.0, 0.0);
    planeSource->SetPoint2(-5.0, 5.0, 0.0);

    vtkNew<vtkPolyDataMapper> floorMapper;
    floorMapper->SetInputConnection(planeSource->GetOutputPort());

    // --- STEP 4: THE ACTOR (THE DYNAMIC TRANSFORM) ---
    vtkNew<vtkActor> floorActor;
    floorActor->SetMapper(floorMapper);

    // This is the "Magic Move." We tell the Actor to slide to 
    // wherever your headset was detected in Step 2.
    floorActor->SetPosition(currentX, currentY, 0.0);

    // Set visuals
    floorActor->GetProperty()->SetColor(0.1, 0.4, 0.1); // Green
    floorActor->GetProperty()->BackfaceCullingOff();   // Visible from both sides

    renderer->AddActor(floorActor);
    renderer->SetBackground(0.2, 0.3, 0.4); // Blue void

    printf("Floor successfully translated to Headset Position: X=%.2f, Y=%.2f\n", currentX, currentY);

    // --- STEP 5: START THE WORLD ---
    interactor->Start();

    return 0;
}