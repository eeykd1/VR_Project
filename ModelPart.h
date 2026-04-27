#ifndef MODELPART_H
#define MODELPART_H

#include <string>

/**
 * @brief THE SHARED LIST OF VARIABLE NAMES (Our Team Agreement)
 * This file contains the exact names for the variables we all need to use.
 * It ensures that when one person's task changes a setting, the other 
 * tasks see that change immediately so the final program doesn't break.
 */
struct ModelPart {
    
    // --- 1. MANAGEMENT VARIABLES (Task 1 & 3) ---

    // The ID of the part. Task 1 (GUI) sets it; Task 2/3 use it to find and load the STL file.
    std::string fileName;

    // The Show/Hide switch. Task 1 (GUI) toggles it; Task 3 (VR) reads it to draw or hide the part.
    bool isVisible = true;


    // --- 2. VISUAL PROPERTY VARIABLES (Task 1 & 3) ---

    // The "Paint." Task 1 (GUI) picks the color; Task 3 (VR) applies it to the 3D model.
    double color[3] = {1.0, 1.0, 1.0};

    // The "Transparency." Task 1 (GUI) sets the slider; Task 3 (VR) makes the model see-through.
    double opacity = 1.0;


    // --- 3. VTK FILTER PARAMETERS (Task 2) ---

    // The "Explode" value. Task 1 (GUI) sets the gap; Task 2 (Filters) uses it for the shrink math.
    double shrinkFactor = 1.0;

    // The "Saw Blade" position. Task 1 (GUI) sets the height; Task 2 (Filters) cuts the model here.
    double clipLevel = 0.5;

    // The "Step Sequence." Task 1 (GUI) picks the order; Task 2 (Filters) decides which math comes first.
    int filterOrder = 0; 


    // --- 4. ANIMATION VARIABLES (Task 4) ---

    // The "Spin" switch. Task 4 (Animation) triggers it; Task 3 (VR) performs the rotation frames.
    bool isRotating = false;
};

#endif // MODELPART_H
