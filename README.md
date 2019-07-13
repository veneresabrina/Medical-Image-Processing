# Medical-Image-Processing
Code for Matlab and ITK, segmentation of the liver using the graph cut theory and 3D rendering of abdomen and liver

The set of Images was composed by 199 CT Images in DICOM format.
In Matlab the liver is segmented using a tecnique inspired by the graph cut theory.
This technique takes into account an initial seed (pixel of the image) and its 8 neighbors.
The pixel of the image is compared to the matrix of 9 pixels (seeds) by exploiting a check on the energy function and on the intensity of the pixel. When the pixel of the image is voted as part of the ROI (liver) by most of the seeds then it is classified as liver.
A set of initial images is segmented with this graph cut tecnique and the result of the segmentation is propagated to the other slices.
The propagation technique simply checks if the intensity of the image is coherent with the previous value.
The obtained liver is rendered with RayCasting tecnique in VTK as well as the complete abdoment and a multiplanar reconstruction of the abdomen in the three main planes.

The MPR is not syncronized, this means that when the user scrolls on a plane changing the slice currently seen, the others do not change.

Only main pieces of code were provided in this repository (not the full working folder with the matlab user interface and the images).
