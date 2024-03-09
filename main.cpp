#include <vector>
#include <iostream>
#include <string>

// all from ML
#include "../graph.h"
#include "../matrix.h"
#include "../dataset.h"
#include "../PCA.h"

using namespace std;





vector<Mat> getImageChannels(const string& src) {
	olc::Sprite sprite;

	if (!sprite.LoadFromFile(src)) {
		return vector<Mat>();
	}

	int w = sprite.width;
	int h = sprite.height;

	vector<Mat> channels(3, Mat(h, w));

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			olc::Pixel pixel = sprite.GetPixel(x, y);

			channels[0][y][x] = pixel.r;
			channels[1][y][x] = pixel.g;
			channels[2][y][x] = pixel.b;
		}
	}

	return channels;
}


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
	vector<Mat> channels = getImageChannels("images/city.jpg");
//	vector<Mat> channels = { getImageGrayscale("images/city.jpg") };

	// build the dataset. Each row of the image will be a data point
	vector<Dataset> imgs(channels.size());
	for (size_t n = 0; n < channels.size(); ++n) {
		for (size_t i = 0; i < channels[n].row; ++i) {
			imgs[n].add(DataPoint(channels[n][i]));
		}
	}

	vector<PCA> models(channels.size());
	for (size_t n = 0; n < channels.size(); ++n) {
		models[n].fit(imgs[n], 0.97); // keep 97% of explained variance
	}


	// in reality we would only store the transformed image and the PCA model
	vector<Dataset> compressed(channels.size());
	for (size_t n = 0; n < channels.size(); ++n) {
		compressed[n] = models[n].toOriginalSpace(models[n].transform(imgs[n]));

		// clamp the values because PCA sometimes makes it out of bound
		compressed[n].clampX(0, 255);
	}


	// display original image for comparison
	olc::Sprite sprite(channels[0].col, channels[0].row);
	for (size_t i = 0; i < channels[0].row; ++i) {
		for (size_t j = 0; j < channels[0].col; ++j) {

			int8_t r = imgs[0 % imgs.size()][i].x[j];
			int8_t g = imgs[1 % imgs.size()][i].x[j];
			int8_t b = imgs[2 % imgs.size()][i].x[j];

			sprite.SetPixel(j, i, olc::Pixel(r, g, b));
		}
	}
	graph.addImage(&sprite, 1, 1, 2, 2);
	
	olc::Sprite spriteCompressed(channels[0].col, channels[0].row);
	for (size_t i = 0; i < channels[0].row; ++i) {
		for (size_t j = 0; j < channels[0].col; ++j) {

			int8_t r = compressed[0 % compressed.size()][i].x[j];
			int8_t g = compressed[1 % compressed.size()][i].x[j];
			int8_t b = compressed[2 % compressed.size()][i].x[j];

			spriteCompressed.SetPixel(j, i, olc::Pixel(r, g, b));
		}
	}

	// display the compressed image
	graph.addImage(&spriteCompressed, 2.1, 1, 3.1, 2);

	// waits untill the user closes the graph
	graph.waitFinish();

	return 0;
}
