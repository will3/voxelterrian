#pragma once

template <typename T>
class Field3 {
private:
	std::vector<T> data;
public:
	int size = 32;
	Field3(int size) {
		this->size = size;
		data.resize(size * size * size);
	}
	T get(int i, int j, int k) {
		int index = i * size * size + j * size + k;
		return data[index];
	}

	void set(int i, int j, int k, T v) {
		int index = i * size * size + j * size + k;
		data[index] = v;
	}

	T sample(float i, float j, float k) {
		int min_i = floor(i);
		int min_j = floor(j);
		int min_k = floor(k);
		int max_i = min_i + 1;
		int max_j = min_j + 1;
		int max_k = min_k + 1;

		float ratio_i = min_i - i;
		float ratio_j = min_j - j;
		float ratio_k = min_k - k;

		float a = get(min_i, min_j, min_k);
		float b = get(max_i, min_j, min_k);
		float c = get(min_i, min_j, max_k);
		float d = get(max_i, min_j, max_k);
		float e = get(min_i, max_j, min_k);
		float f = get(max_i, max_j, min_k);
		float g = get(min_i, max_j, max_k);
		float h = get(max_i, max_j, max_k);

		float v = (a * ratio_i + b * (1 - ratio_i)) * ratio_k + (c * ratio_i + d * (1 - ratio_i)) * (1 - ratio_k) * ratio_j +
			(e * ratio_i + f * (1 - ratio_i)) * ratio_k + (g * ratio_i + h * (1 - ratio_i)) * (1 - ratio_k) * (1 - ratio_j);

		return v;
	}
};