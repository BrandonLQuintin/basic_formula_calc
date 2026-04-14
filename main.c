#include <stdio.h>
#include <math.h>

// PI definition for standard math
#define PI 3.14159265359

int main() {
    // ==========================================
    // 1. PHYSICAL CONSTANTS (The Scanner Setup)
    // ==========================================
    // These remain fixed for the entire scan.
    
    // B: Distance from camera to laser in millimeters (Baseline)
    // TI-84: 100 -> B
    double B = 100.0; 
    
    // alpha_deg: Angle of the laser pointing inward (Degrees)
    // TI-84: Ensure your calculator is in DEGREE mode! Press [MODE], select [DEGREE].
    // TI-84: 20 -> A
    double alpha_deg = 20.0; 
    
    // Convert degrees to radians for C's math functions
    double alpha_rad = alpha_deg * (PI / 180.0); 
    
    // f: Focal length of the camera lens in mm
    // TI-84: 8 -> F
    double f = 8.0; 
    
    // p: Physical size of a single pixel on the sensor in mm (Pixel Pitch)
    // OV7670 QVGA: 0.0072 mm
    // TI-84: 0.0072 -> P
    double p = 0.0072; 
    
    // X_center: The middle column of the camera sensor (320 / 2 for QVGA)
    // TI-84: 160 -> C
    double X_center = 160.0; 


    // ==========================================
    // 2. INPUT VARIABLES (From a single laser strike)
    // ==========================================
    // This is the data we receive from the microcontroller.
    
    // X_c: The sub-pixel column where the laser is brightest
    // TI-84: 400.5 -> X
    double X_c = 400.5; 
    
    // Row: The current horizontal row being processed
    // TI-84: 200 -> R
    double Row = 200.0; 
    
    // Step: Current motor position (out of 4096 steps per revolution)
    // TI-84: 1024 -> S
    double Step = 1024.0; 


    // ==========================================
    // 3. THE MATHEMATICAL PIPELINE
    // ==========================================

    // Step 1: Camera Depth (r) - Distance to the laser strike
    // TI-84 Formula: (F * B) / ((X - C) * P + F * tan(A)) -> Y_{depth}
    // TI-84: (8 * 100) / ((400.5 - 320) * 0.0036875 + 8 * tan(30)) -> Y_{depth}
    // Note: I used Y_{depth} here because 'R' is already used for Row.
    //
    //              f * B
    //   r = -----------------------
    //       (Xc - X_center) * p + f * tan(a)
    //
    double r = (f * B) / ((X_c - X_center) * p + f * tan(alpha_rad));
    
    // Step 2: Axis Depth (D) - Distance to turntable center
    // TI-84 Formula: B / tan(A) -> D
    // TI-84: 100 / tan(30) -> D
    //
    //       B
    //   D = -----
    //       tan(a)
    //
    double D = B / tan(alpha_rad);
    
    // Step 3: Radial Distance from Axis (r_axis)
    // TI-84 Formula: (D - Y_{depth}) / cos(A) -> V
    // TI-84: (173.21 - Y_{depth}) / cos(30) -> V
    //
    //            D - r
    //   r_axis = -----
    //            cos(a)
    //
    double r_axis = (D - r) / cos(alpha_rad);
    
    // Step 4: True Height (Y) - Account for perspective
    // TI-84 Formula: (R - 120) * P * (Y_{depth} / F) -> Y_{final}
    // TI-84: (200 - 120) * 0.0036875 * (Y_{depth} / 8) -> Y_{final}
    //
    //   Y = (Row - 120) * p * (r / f)
    //
    double Y = (Row - 120.0) * p * (r / f);
    
    // Step 5: Cylindrical to Cartesian Projection (X_final, Z_final) 
    // TI-84: (S / 4096) * 2 * pi -> T
    // TI-84: (1024 / 4096) * 2 * pi -> T
    // Note: Even if TI-84 is in Degree mode, pi is inherently a radian concept. 
    // For the TI-84 to calculate sin/cos correctly in Degree mode, convert T to degrees: 
    // TI-84: (S / 4096) * 360 -> T
    // TI-84: (1024 / 4096) * 360 -> T
    //
    //            Step
    //   theta = ------ * 2 * PI
    //            4096
    //
    //   X_final = r_axis * cos(theta)
    //   Z_final = r_axis * sin(theta)
    //
    double theta = (Step / 4096.0) * 2.0 * PI;
    
    // TI-84: V * cos(T) -> X_{final}
    // TI-84: V * sin(T) -> Z_{final}
    // Note: T should be 90 degrees from step 5, so cos(90)=0, sin(90)=1
    double X_final = r_axis * cos(theta);
    double Z_final = r_axis * sin(theta);


    // ==========================================
    // 4. OUTPUT RESULTS
    // ==========================================
    printf("--- Input Data ---\n");
    printf("Laser detected at Pixel X: %.1f, Row Y: %.0f\n", X_c, Row);
    printf("Turntable Position: Step %.0f of 4096\n\n", Step);
    
    printf("--- Intermediate Math ---\n");
    printf("Camera Depth (r):       %.2f mm\n", r);
    printf("Axis Depth (D):         %.2f mm\n", D);
    printf("Radial Distance (r_ax): %.2f mm\n\n", r_axis);
    
    printf("--- Final 3D Coordinate ---\n");
    printf("X: %.2f mm\n", X_final);
    printf("Y: %.2f mm\n", Y);
    printf("Z: %.2f mm\n", Z_final);

    return 0;
}