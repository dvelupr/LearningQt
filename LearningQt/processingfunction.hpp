#pragma once

enum class ProcessingFunction
{
	// Image Correction
	/*
	BRIGHTNESS,
	CONTRAST,
	GAMMA,
	*/

	// Smooth or Blur Image
	BILATERAL_FILTER,
	GAUSSIAN_BLUR,
	HOMOGENEOUS_BLUR,
	MEDIAN_BLUR,

	// Morphology
	EROSION,
	DILATION,

	// Non-Photorealistic Rendering
	NORMALIZED_CONVOLUTION_FILTER,
	RECURSIVE_FILTER,
	DETAIL_ENHANCEMENT,
	PENCIL_SKETCH,
	COLOR_PENCIL_DRAWING,
	STYLIZATION,

	// Edge Detection
	CANNY_EDGE_DETECTION,
	SOBEL_EDGE_DETECTION,

	// Miscellaneous
	ADAPTIVE_THRESHOLD,
	BINARY_THRESHOLD,
	CONTOURS,
	CONVEX_HULL,
	CORNER_DETECTION,
	HOUGH_CIRCLES,
	HOUGH_LINES,
	LAPLACIAN,
	EQUALIZE_HISTOGRAM,
	OBJECT_DETECTION
};
