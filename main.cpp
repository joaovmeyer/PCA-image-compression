#include <iostream>
#include <string>

// all from ML
#include "../graph.h"
#include "../matrix.h"
#include "../dataset.h"
#include "../PCA.h"

using namespace std;

int toGrayscale(int r, int g, int b) {
	return static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b);
}

Mat getImageGrayscale(const string& src) {
	olc::Sprite sprite;

	if (!sprite.LoadFromFile(src)) {
		return Mat();
	}

	int w = sprite.width;
	int h = sprite.height;

	Mat img(h, w);

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			olc::Pixel pixel = sprite.GetPixel(x, y);

			img[y][x] = toGrayscale(pixel.r, pixel.g, pixel.b);
		}
	}

	return img;
}



int main() {

	Graph graph;

	// load the image
	Mat grayscale = getImageGrayscale("images/Lenna.png");

	// build the dataset. Each row of the image will be a data point
	Dataset img;
	for (size_t i = 0; i < grayscale.row; ++i) {
		img.add(DataPoint(grayscale[i]));
	}

	PCA model;
	model.fit(img, 0.97); // keep 97% of explained variance

	// in reality we would only store the transformed image and the PCA model
	Dataset compressed = model.toOriginalSpace(model.transform(img));

	olc::Sprite sprite(grayscale.col, grayscale.row);
	for (size_t i = 0; i < grayscale.row; ++i) {
		for (size_t j = 0; j < grayscale.col; ++j) {

			double color = compressed[i].x[j];

			sprite.SetPixel(j, i, olc::Pixel(color, color, color));
		}
	}

	// display the compressed image
	graph.addImage(&sprite, 0, 0, 1, 1);

	// waits untill the user closes the graph
	graph.waitFinish();

	return 0;
}
