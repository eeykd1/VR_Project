#include <vtkNew.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <string>

// ==========================================================
// STEP 4: ANIMATION CALLBACK
// This class is called every frame by VTK.
// It checks isRotating and spins the actor if true.
// ==========================================================
class RotationCallback : public vtkCommand {
public:
    static RotationCallback* New() { return new RotationCallback; }

    // --- Agreed variable from ModelPart.h ---
    bool isRotating = true;

    vtkActor* modelActor = nullptr;

    void Execute(vtkObject* caller, unsigned long, void*) override {
        if (isRotating && modelActor) {
            // Rotate the model 1 degree per frame around the Y axis
            modelActor->RotateY(1.0);

            // Re-render the scene
            vtkRenderWindow* rw = static_cast<vtkRenderWindow*>(caller);
            rw->Render();
        }
    }
};

int main() {
    // --- STEP 1: INITIALIZE VR SYSTEM ---
    vtkNew<vtkOpenVRRenderer> renderer;
    vtkNew<vtkOpenVRRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkOpenVRRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);
    renderWindow->Initialize();

    // --- STEP 2: DYNAMIC POSITION CHECK ---
    double headPos[3];
    renderer->GetActiveCamera()->GetPosition(headPos);
    double currentX = headPos[0];
    double currentZ = headPos[2];

    // --- STEP 3: FLOOR SOURCE & MAPPER ---
    vtkNew<vtkPlaneSource> planeSource;
    planeSource->SetOrigin(-5.0, 0.0, -5.0);
    planeSource->SetPoint1(5.0, 0.0, -5.0);
    planeSource->SetPoint2(-5.0, 0.0, 5.0);
    vtkNew<vtkPolyDataMapper> floorMapper;
    floorMapper->SetInputConnection(planeSource->GetOutputPort());

    // --- STEP 4: FLOOR ACTOR ---
    vtkNew<vtkActor> floorActor;
    floorActor->SetMapper(floorMapper);
    floorActor->SetPosition(currentX, 0.0, currentZ);
    floorActor->GetProperty()->SetColor(0.1, 0.4, 0.1); // Green
    floorActor->GetProperty()->BackfaceCullingOff();
    renderer->AddActor(floorActor);
    renderer->SetBackground(0.2, 0.3, 0.4); // Blue void

    // ==========================================================
    // STEP 2: LOAD STL MODEL AND PLACE IT IN THE VR WORLD
    // ==========================================================
    std::string fileName = "D:/Uni/25-26/Software Development Group Design Project/keene/test_cube.stl";
    bool isVisible = true;
    double color[3] = { 1.0, 0.0, 0.0 }; // Red
    double opacity = 1.0;

    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName(fileName.c_str());
    stlReader->Update();

    vtkSmartPointer<vtkPolyDataMapper> modelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    modelMapper->SetInputConnection(stlReader->GetOutputPort());

    vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();
    modelActor->SetMapper(modelMapper);
    modelActor->SetVisibility(isVisible ? 1 : 0);
    modelActor->GetProperty()->SetColor(color[0], color[1], color[2]);
    modelActor->GetProperty()->SetOpacity(opacity);
    modelActor->SetPosition(currentX, 1.0, currentZ - 1.5);
    modelActor->SetScale(0.01, 0.01, 0.01);
    renderer->AddActor(modelActor);

    // ==========================================================
    // STEP 4: SET UP THE ROTATION CALLBACK
    // Attach to render event so it fires every frame
    // ==========================================================
    vtkSmartPointer<RotationCallback> callback = vtkSmartPointer<RotationCallback>::New();
    callback->modelActor = modelActor;
    callback->isRotating = true; // Set to false to stop rotation

    // Attach to render event instead of timer
    renderWindow->AddObserver(vtkCommand::RenderEvent, callback);

    printf("STL model loaded: %s\n", fileName.c_str());
    printf("Floor translated to Headset Position: X=%.2f, Z=%.2f\n", currentX, currentZ);

    // --- START THE WORLD ---
    interactor->Start();
    return 0;
}