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
    double B = 100.0; 
    
    // alpha_deg: Angle of the laser pointing inward (Degrees)
    double alpha_deg = 20.0; 
    
    // Convert degrees to radians for C's math functions
    double alpha_rad = alpha_deg * (PI / 180.0); 
    
    // f: Focal length of the camera lens in mm
    double f = 17.5; 
    
    // p: Physical size of a single pixel on the sensor in mm (Pixel Pitch)
    // OV7670 QVGA: 0.0072 mm
    double p = 0.0072; 
    
    // X_center: The middle column of the camera sensor (320 / 2 for QVGA)
    double X_center = 160.0; 


    // ==========================================
    // 2. INPUT VARIABLES (From a single laser strike)
    // ==========================================
    // This is the data we receive from the microcontroller.
    
    // X_c: The sub-pixel column where the laser is brightest (QVGA: 0-319)
    double X_c = 178; 
    
    // Row: The current horizontal row being processed
    double Row = 200.0; 
    
    // Step: Current motor position (out of 4096 steps per revolution)
    double Step = 0; 


    // ==========================================
    // 3. THE MATHEMATICAL PIPELINE
    // ==========================================

    // Step 1: Camera Depth (r) - Distance to the laser strike
    //
    //              f * B
    //   r = -----------------------
    //       (Xc - X_center) * p + f * tan(a)
    //
    double r = (f * B) / ((X_c - X_center) * p + f * tan(alpha_rad));
    
    // Step 2: Axis Depth (D) - Distance to turntable center
    //
    //       B
    //   D = -----
    //       tan(a)
    //
    double D = B / tan(alpha_rad);
    
    // Step 3: Radial Distance from Axis (r_axis)
    //
    //            D - r
    //   r_axis = -----
    //            cos(a)
    //
    double r_axis = (D - r) / cos(alpha_rad);
    
    // Step 4: True Height (Y) - Account for perspective
    //
    //   Y = (Row - 120) * p * (r / f)
    //
    double Y = -(Row - 120.0) * p * (r / f);
    
    // Step 5: Cylindrical to Cartesian Projection (X_final, Z_final) 
    //
    //            Step
    //   theta = ------ * 2 * PI
    //            4096
    //
    //   X_final = r_axis * cos(theta)
    //   Z_final = r_axis * sin(theta)
    //
    double theta = (Step / 4096.0) * 2.0 * PI;
    
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