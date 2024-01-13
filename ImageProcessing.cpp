#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#pragma once
#ifndef CONTRASTMODIFICATION_H
#define CONTRASTMODIFICATION_H
#include "ContrastModification.h"
#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <bits/stdc++.h>
using namespace std;
#ifndef DITHERING_H
#define DITHERING_H

void Histogram_Equalization(const olc::Sprite* src, olc::Sprite* dest) {
	std::copy(src->pColData.begin(), src->pColData.end(), dest->pColData.begin());

	float numPixels = float(src->height) * float(src->width);
	float alpha = 255.0f / numPixels;

	vector<vector<int>> histogram(3, vector<int>(256, 0));

	olc::vi2d vPixel;
	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {


			olc::Pixel px = src->GetPixel(vPixel);

			histogram[0][px.r]++;
			histogram[1][px.g]++;
			histogram[2][px.b]++;
		}
	}
	vector<vector<float>> lookupTable(3, vector<float>(256, 0.0f));

	lookupTable[0][0] = alpha * histogram[0][0];
	lookupTable[1][0] = alpha * histogram[1][0];
	lookupTable[2][0] = alpha * histogram[2][0];


	for (int i = 1; i < 256; i++) {
		lookupTable[0][i] = lookupTable[0][i - 1] + alpha * histogram[0][i];
		lookupTable[1][i] = lookupTable[1][i - 1] + alpha * histogram[1][i];
		lookupTable[2][i] = lookupTable[2][i - 1] + alpha * histogram[2][i];
	}

	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel px = dest->GetPixel(vPixel);

			px.r = lookupTable[0][px.r];
			px.g = lookupTable[1][px.g];
			px.b = lookupTable[2][px.b];


			dest->SetPixel(vPixel, px);
		}
	}

}
void Generate_Gaussian_Kernel(vector<vector<float>>& kernel, float stdDev = 1.0f) {
	int dim = kernel.size() / 2;

	float kernelSum = 0.0f;
	float pi = 3.14159265358979323846f;

	float variance = stdDev * stdDev;
	for (int i = -dim; i <= dim; i++) {
		for (int j = -dim; j <= dim; j++) {
			kernel[i + dim][j + dim] = expf(-(float(i * i + j * j)) / (2.0f * variance)) / (2.0f * pi * variance);
			kernelSum += kernel[i + dim][j + dim];
		}
	}
	for (int i = -dim; i <= dim; i++) {
		for (int j = -dim; j <= dim; j++) {
			kernel[i + dim][j + dim] /= kernelSum;
		}
	}
}

void Convolution(const olc::Sprite* src, olc::Sprite* dest, vector<vector<float>> &kernel) {

	int nDim = kernel.size();
	int dim = nDim / 2;
	std::copy(src->pColData.begin(), src->pColData.end(), dest->pColData.begin());

	olc::vi2d vPixel;
	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel op = dest->GetPixel(vPixel);

			olc::Pixel cp = op;
			float conv_sum[3] = { 0.0f, 0.0f, 0.0f };
			for (int i = -dim; i <= dim; i++) {
				for (int j = -dim; j <= dim; j++) {
					olc::vi2d vOffset = { i,j };
					olc::Pixel tp = src->GetPixel(vPixel + vOffset);
					conv_sum[0] += float(tp.r) * kernel[i + dim][j + dim];
					conv_sum[1] += float(tp.g) * kernel[i + dim][j + dim];
					conv_sum[2] += float(tp.b) * kernel[i + dim][j + dim];
				}
			}

			dest->SetPixel(vPixel, olc::Pixel(std::clamp(int(conv_sum[0]), 0, 255), std::clamp(int(conv_sum[1]), 0, 255), std::clamp(int(conv_sum[2]), 0, 255)));
		}
	}
}


void Sobel_Edge_Detection(const olc::Sprite* src, olc::Sprite* dest) {

	vector<vector<float>> vertical_kernel =
	{
			{-1.0f, 0.0f, +1.0f},
			{-2.0f, 0.0f, +2.0f},
			{-1.0f, 0.0f, +1.0f}
	};
	Convolution(src, dest, vertical_kernel);

	std::unique_ptr<olc::Sprite> m_pTemp;
	m_pTemp = std::make_unique<olc::Sprite>(1280, 960);


	vector<vector<float>> horizontal_kernel = {
		{-1.0f, -2.0f, -1.0f},
		{ 0.0f, 0.0f, 0.0f},
		{+1.0f, +2.0f, +1.0f}
	};
	Convolution(src, m_pTemp.get(), horizontal_kernel);

	olc::vi2d vPixel;
	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel op = dest->GetPixel(vPixel);

			olc::Pixel ep = m_pTemp.get()->GetPixel(vPixel);

			op.r = (op.r + ep.r) / 2.0f;
			op.g = (op.g + ep.g) / 2.0f;
			op.b = (op.b + ep.b) / 2.0f;

			dest->SetPixel(vPixel, olc::Pixel(std::clamp(int(op.r), 0, 255), std::clamp(int(op.g), 0, 255), std::clamp(int(op.b), 0, 255)));
		}
	}
}
void JJN_Dithering(const olc::Sprite* src, olc::Sprite* dest, std::function<olc::Pixel(const olc::Pixel)> funcQuantize) {
	std::copy(src->pColData.begin(), src->pColData.end(), dest->pColData.begin());

	olc::vi2d vPixel;
	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel op = dest->GetPixel(vPixel);
			olc::Pixel qp = funcQuantize(op);
			int32_t qError[3] = {
				op.r - qp.r,
				op.g - qp.g,
				op.b - qp.b
			};

			dest->SetPixel(vPixel, qp);

			auto UpdatePixel = [&vPixel, &qError, &dest](const olc::vi2d& vOffset, const float fErrorBias) {
				olc::Pixel p = dest->GetPixel(vPixel + vOffset);
				int32_t k[3] = {
					p.r + int32_t(float(qError[0] * fErrorBias)),
					p.g + int32_t(float(qError[1] * fErrorBias)),
					p.b + int32_t(float(qError[2] * fErrorBias))
				};

				dest->SetPixel(vPixel + vOffset, olc::Pixel(std::clamp(k[0], 0, 255), std::clamp(k[1], 0, 255), std::clamp(k[2], 0, 255)));
			};

			UpdatePixel({ 0,2 }, 5.0f / 48.0f);
			UpdatePixel({ 2,0 }, 5.0f / 48.0f);
			UpdatePixel({ 1,0 }, 7.0f / 48.0f);
			UpdatePixel({ 0,1 }, 7.0f / 48.0f);
			UpdatePixel({ -1,1 }, 5.0f / 48.0f);
			UpdatePixel({ 1,1 }, 5.0f / 48.0f);
			UpdatePixel({ 2,1 }, 3.0f / 48.0f);
			UpdatePixel({ -2,1 }, 3.0f / 48.0f);
			UpdatePixel({ -2,2 }, 1.0f / 48.0f);
			UpdatePixel({ 2,2 }, 1.0f / 48.0f);
			UpdatePixel({ 1,2 }, 3.0f / 48.0f);
			UpdatePixel({ -1,2 }, 3.0f / 48.0f);
		}
	}

}




void Floyd_Steinberg_Dithering(const olc::Sprite* src, olc::Sprite* dest, std::function<olc::Pixel(const olc::Pixel)> funcQuantize) {
	std::copy(src->pColData.begin(), src->pColData.end(), dest->pColData.begin());

	olc::vi2d vPixel;

	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel op = dest->GetPixel(vPixel);
			olc::Pixel qp = funcQuantize(op);
			int32_t qError[3] = {
				op.r - qp.r,
				op.g - qp.g,
				op.b - qp.b
			};

			dest->SetPixel(vPixel, qp);

			auto UpdatePixel = [&vPixel, &qError, &dest](const olc::vi2d& vOffset, const float fErrorBias) {
				olc::Pixel p = dest->GetPixel(vPixel + vOffset);
				int32_t k[3] = { p.r + int32_t(float(qError[0] * fErrorBias)),
					p.g + int32_t(float(qError[1] * fErrorBias)),
					p.b + int32_t(float(qError[2] * fErrorBias))
				};

				dest->SetPixel(vPixel + vOffset, olc::Pixel(std::clamp(k[0], 0, 255), std::clamp(k[1], 0, 255), std::clamp(k[2], 0, 255)));
			};

			UpdatePixel({ 1,0 }, 7.0f / 16.0f);
			UpdatePixel({ -1,1 }, 3.0f / 16.0f);
			UpdatePixel({ 0,1 }, 5.0f / 16.0f);
			UpdatePixel({ 1,1 }, 1.0f / 16.0f);
		}
	}
}

void Stucki_Dithering(const olc::Sprite* src, olc::Sprite* dest, std::function<olc::Pixel(const olc::Pixel)> funcQuantize) {
	std::copy(src->pColData.begin(), src->pColData.end(), dest->pColData.begin());

	olc::vi2d vPixel;
	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel op = dest->GetPixel(vPixel);
			olc::Pixel qp = funcQuantize(op);
			int32_t qError[3] = {
				op.r - qp.r,
				op.g - qp.g,
				op.b - qp.b
			};

			dest->SetPixel(vPixel, qp);

			auto UpdatePixel = [&vPixel, &qError, &dest](const olc::vi2d& vOffset, const float fErrorBias) {
				olc::Pixel p = dest->GetPixel(vPixel + vOffset);
				int32_t k[3] = { p.r + int32_t(float(qError[0] * fErrorBias)),
					p.g + int32_t(float(qError[1] * fErrorBias)),
					p.b + int32_t(float(qError[2] * fErrorBias))
				};

				dest->SetPixel(vPixel + vOffset, olc::Pixel(std::clamp(k[0], 0, 255), std::clamp(k[1], 0, 255), std::clamp(k[2], 0, 255)));
			};

			UpdatePixel({ 2,0 }, 4.0f / 42.0f);
			UpdatePixel({ 1,0 }, 8.0f / 42.0f);
			UpdatePixel({ 0,2 }, 4.0f / 42.0f);
			UpdatePixel({ 0,1 }, 8.0f / 42.0f);
			UpdatePixel({ -1,1 }, 4.0f / 42.0f);
			UpdatePixel({ 1,1 }, 4.0f / 42.0f);
			UpdatePixel({ 2,1 }, 2.0f / 42.0f);
			UpdatePixel({ -2,1 }, 2.0f / 42.0f);
			UpdatePixel({ -2,2 }, 1.0f / 42.0f);
			UpdatePixel({ 2,2 }, 1.0f / 42.0f);
			UpdatePixel({ 1,2 }, 2.0f / 42.0f);
			UpdatePixel({ -1,2 }, 2.0f / 42.0f);
		}
	}

}




vector<vector<float>> Generate_Threshold_Map(int n=2) {

	vector<vector<float>> mat = { { 0, 2}, { 3, 1} };

	vector<vector<float>> fin_mat;
	for (int i = 0; i < n; i++) {
		// 4 * mat

		vector<vector<float>> new_mat;

		for (int r = 0; r < mat.size(); r++) {
			vector<float> new_vec;
			for (int c = 0; c < mat.size(); c++) {
				new_vec.push_back(4.0f * mat[r][c]);
			}
			new_mat.push_back(new_vec);
		}

		for (int r = 0; r < mat.size(); r++) {
			vector<float> new_vec;
			for (int c = 0; c < mat.size(); c++) {
				new_vec.push_back(new_mat[r][c]);
			}
			fin_mat.push_back(new_vec);
		}
		for (int r = 0; r < mat.size(); r++) {
			for (int c = 0; c < mat.size(); c++) {
				fin_mat[r].push_back(new_mat[r][c] + 2.0f);
			}
		}

		for (int r = 0; r < mat.size(); r++) {
			vector<float> new_vec;
			for (int c = 0; c < mat.size(); c++) {
				new_vec.push_back(new_mat[r][c] + 3.0f);
			}
			fin_mat.push_back(new_vec);
		}

	
		for (int r = mat.size(); r < 2 * mat.size(); r++) {
			for (int c = mat.size(); c < 2 * mat.size(); c++) {
				fin_mat[r].push_back(new_mat[r - mat.size()][c - mat.size()] + 1.0f);
			}
		}

		mat = fin_mat;
		fin_mat.clear();
	}


	float scaling_factor = powf(2.0f, (float)(2 * n + 2));

	for (int i = 0; i < mat.size(); i++) {
		for (int j = 0; j < mat.size(); j++) {

		
			mat[i][j] = mat[i][j] / scaling_factor;
			mat[i][j] -= 0.5f;
		}

	}

	return mat;

}

void Ordered_Dithering(const olc::Sprite* src, olc::Sprite* dest, std::function<olc::Pixel(const olc::Pixel)> funcQuantize, int n=2) {

	vector<vector<float>> threshold_map = Generate_Threshold_Map(3);

	std::copy(src->pColData.begin(), src->pColData.end(), dest->pColData.begin());

	float rVal = 255.0f / 4.0f;
	int map_size = threshold_map.size();

	olc::vi2d vPixel;
	for (vPixel.y = 0; vPixel.y < src->height; vPixel.y++) {
		for (vPixel.x = 0; vPixel.x < src->width; vPixel.x++) {
			olc::Pixel op = dest->GetPixel(vPixel);

			op.r += rVal * threshold_map[vPixel.y % map_size][vPixel.x % map_size];
			op.g += rVal * threshold_map[vPixel.y % map_size][vPixel.x % map_size];
			op.b += rVal * threshold_map[vPixel.y % map_size][vPixel.x % map_size];

			olc::Pixel qp = funcQuantize(op);


			dest->SetPixel(vPixel, qp);


		}
	}

}


class ImageProcessing : public olc::PixelGameEngine {

	public:
		ImageProcessing() {
			sAppName = "Image Processing";
		}

		olc::TransformedView tv;

		std::unique_ptr<olc::Sprite> m_pImage;
		std::unique_ptr<olc::Sprite> m_pQuantised;
		std::unique_ptr<olc::Sprite> m_pDithered;
		std::unique_ptr<olc::Sprite> m_pBlurred;
		std::unique_ptr<olc::Sprite> m_pEdge;
		std::unique_ptr<olc::Sprite> m_pGray;
		std::unique_ptr<olc::Sprite> m_pHighContrast;
		std::unique_ptr<olc::Sprite> m_pHistEqualized;
		
	public: 

		bool OnUserCreate() override {

			tv.Initialise({ ScreenWidth(), ScreenHeight() });

			cout << "Image Processing!!!" << endl;
			m_pImage = std::make_unique<olc::Sprite>("image.jpg");
			m_pQuantised = std::make_unique<olc::Sprite>(m_pImage->width, m_pImage->height);
			m_pDithered = std::make_unique<olc::Sprite>(m_pImage->width, m_pImage->height);
			m_pBlurred = std::make_unique<olc::Sprite>(m_pImage->width, m_pImage->height);
			m_pEdge = std::make_unique<olc::Sprite>("image.jpg");
			m_pGray = std::make_unique<olc::Sprite>(m_pImage->width, m_pImage->height);

			m_pHighContrast = std::make_unique<olc::Sprite>("low_contrast_rgb.jpeg");
			m_pHistEqualized = std::make_unique<olc::Sprite>(m_pHighContrast->width, m_pHighContrast->height);

			auto Convert_RGB_to_Grayscale = [](const olc::Pixel in) {
				uint8_t grayscale = uint8_t(0.2162f * float(in.r) + (0.7152f) * float(in.g) + 0.0722f * float(in.b));

				return olc::Pixel(grayscale, grayscale, grayscale);
			};

		

			auto Quantize_Grayscale_1bit = [](const olc::Pixel in) {
				return in.r < 128 ? olc::BLACK : olc::WHITE;
			};

			auto Quantize_RGB_nbit = [](const olc::Pixel in) {
				constexpr int nBits = 2;
				constexpr float fLevels = (1 << nBits) - 1;
				uint8_t cr = uint8_t(std::clamp(std::round(float(in.r) / 255.0f * fLevels) / fLevels * 255.0f, 0.0f, 255.0f));
				uint8_t cb = uint8_t(std::clamp(std::round(float(in.g) / 255.0f * fLevels) / fLevels * 255.0f, 0.0f, 255.0f));
				uint8_t cg = uint8_t(std::clamp(std::round(float(in.b) / 255.0f * fLevels) / fLevels * 255.0f, 0.0f, 255.0f));
				return olc::Pixel(cr, cb, cg);
			};

			std::transform(m_pImage->pColData.begin(), m_pImage->pColData.end(), m_pQuantised->pColData.begin(), Quantize_RGB_nbit);


			Floyd_Steinberg_Dithering(m_pImage.get(), m_pDithered.get(), Quantize_RGB_nbit);
			
			vector<vector<float>> kernel(7, vector<float>(7, 0.0f));
			Generate_Gaussian_Kernel(kernel, 7.0f);
			Convolution(m_pImage.get(), m_pBlurred.get(), kernel);

			std::transform(m_pEdge->pColData.begin(), m_pEdge->pColData.end(), m_pGray->pColData.begin(), Convert_RGB_to_Grayscale);
			Sobel_Edge_Detection(m_pGray.get(), m_pEdge.get());

			Histogram_Equalization(m_pHighContrast.get(), m_pHistEqualized.get());
			

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override {

			tv.HandlePanAndZoom();

			Clear(olc::BLACK);

			if (GetKey(olc::Key::Q).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pQuantised.get());
			else if (GetKey(olc::Key::W).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pDithered.get());
			else if (GetKey(olc::Key::E).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pBlurred.get());
			else if (GetKey(olc::Key::R).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pEdge.get());
			else if (GetKey(olc::Key::T).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pGray.get());
			else if (GetKey(olc::Key::Y).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pHistEqualized.get());
			else if (GetKey(olc::Key::U).bHeld)
				tv.DrawSprite({ 0, 0 }, m_pHighContrast.get());
			else
				tv.DrawSprite({ 0,0 }, m_pImage.get());
			return true;
		}
};


int main() {
	ImageProcessing demo;

	if (demo.Construct(1280, 960, 1, 1))
		demo.Start();

	return 0;
}
