/** @file */
#pragma once
#include "WindowsCharacter.h"
#include <stdexcept>


/** @brief Class that represents a 2dMatrix
 *  @author Eryk Kściuczyk
 *  @date June 2021
*/
template <typename T>
class Matrix2D
{
	size_t cols, rows;
	T *arr;

public:
	Matrix2D(size_t _rows, size_t _cols, bool zeroArr = false) : cols(_cols), rows(_rows), arr(new T[_cols * _rows])
	{
		if (zeroArr)
			reset();
	};

	// this array has to be the size of _rows*_cols
	Matrix2D(size_t _rows, size_t _cols, const T _arr[]) : cols(_cols), rows(_rows)
	{
		arr = new T[rows * cols];
		for (size_t i = 0; i < rows * cols; i++)
		{
			arr[i] = _arr[i];
		}
	};

	Matrix2D(const Matrix2D<T> &m) : cols(m.cols), rows(m.rows), arr(new T[m.rows * m.cols])
	{
		for (size_t i = 0; i < m.rows * m.cols; i++)
		{
			arr[i] = m.arr[i];
		}
	}

	Matrix2D(Matrix2D &&m) noexcept : cols(m.cols), rows(m.rows), arr(m.arr)
	{
		m.arr = nullptr;
		m.rows = 0;
		m.cols = 0;
	}

	void reset()
	{
		// returns all members of arr to default values
		for (size_t i = 0; i < (cols * rows); i++)
		{
			T v = T();
			arr[i] = v;
		}
	}

	T &getAt(size_t row, size_t col) const
	{
		if (row >= rows)
		{
			throw std::out_of_range("Row index out of bounds");
		}
		else if (col >= cols)
		{
			throw std::out_of_range("Column index out of bounds");
		}
		else
		{
			return arr[row * cols + col];
		}
	};

	T &operator()(size_t row, size_t col) const
	{
		return getAt(row, col);
	};

	void setAt(size_t row, size_t col, T val)
	{
		if (row >= rows)
		{
			throw std::out_of_range("Row index out of bounds");
		}
		else if (col >= cols)
		{
			throw std::out_of_range("Column index out of bounds");
		}
		else
		{
			arr[row * cols + col] = val;
		}
	};

	size_t colCount() const
	{
		return cols;
	};

	size_t rowCount() const
	{
		return rows;
	};

	Matrix2D<T> *getRow(const size_t row) const
	{
		Matrix2D<T> *m = new Matrix2D<T>(1, cols);
		for (size_t col = 0; col < cols; col++)
		{
			m->setAt(0, col, getAt(row, col));
		}
		return m;
	}

	T *getRowPtr(const size_t row) const
	{
		return arr + row * colCount();
	}

	void setRow(const size_t row, const T values[])
	{
		for (size_t col = 0; col < cols; col++)
		{
			setAt(row, col, values[col]);
		}
	}

	Matrix2D<T> *getCol(const size_t col) const
	{
		Matrix2D<T> *m = new Matrix2D<T>(rows, 1);
		for (size_t row = 0; row < rows; row++)
		{
			m->setAt(row, 0, getAt(row, col));
		}
		return m;
	}

	void setCol(const size_t col, const T values[])
	{
		for (size_t row = 0; row < rows; row++)
		{
			setAt(row, col, values[row]);
		}
	};

	void rotateRight()
	{
		rotate(1);
	}

	void rotateLeft()
	{
		rotate(-1);
	}

	void rotate(short direction = 1)
	{
		// direction 1 - clockwise
		// direction -1 - anticlockwise
		size_t startingIndex = 0;
		if (direction == 1)
		{
			startingIndex = rows - 1;
		}
		else if (direction == -1)
		{
			startingIndex = 0;
		}
		else
		{
			throw std::invalid_argument("Direction can only be 1 or -1");
		}

		Matrix2D<T> m(cols, rows);

		Matrix2D<T> *rowMatrix;
		for (size_t row = 0; row < rows; row++)
		{
			rowMatrix = getRow(row);
			if (direction == -1)
			{
				rowMatrix->reverseColOrder();
			}
			m.setCol(startingIndex - row * direction, rowMatrix->as1DArr());
			delete rowMatrix;
		}
		delete[] arr;
		arr = m.as1DArr();
		m.arr = nullptr;
		rows = m.rows;
		cols = m.cols;
	};

	T *as1DArr() const { return arr; };

	void reverseRowOrder()
	{
		// first row becomes the last row and the last one becomes the first one...
		Matrix2D<T> *rowStart;
		Matrix2D<T> *rowEnd;
		for (size_t row = 0; row < rows / 2; row++)
		{
			size_t rowEndIndex = rows - row - 1;
			rowStart = getRow(row);
			rowEnd = getRow(rowEndIndex);
			setRow(row, rowEnd->as1DArr());
			setRow(rowEndIndex, rowStart->as1DArr());
			delete rowStart;
			delete rowEnd;
		}
	}

	void reverseColOrder()
	{
		// first col becomes the last col and the last one becomes the first one ...
		Matrix2D<T> *colStart;
		Matrix2D<T> *colEnd;
		for (size_t col = 0; col < cols / 2; col++)
		{
			size_t colEndIndex = cols - col - 1;
			colStart = getCol(col);
			colEnd = getCol(colEndIndex);
			setCol(col, colEnd->as1DArr());
			setCol(colEndIndex, colStart->as1DArr());
			delete colStart;
			delete colEnd;
		}
	}

	void operator=(Matrix2D<T> &&m) noexcept
	{
		cols = m.cols;
		rows = m.rows;
		delete[] arr;
		arr = m.arr;
		m.arr = nullptr;
		m.cols = 0;
		m.rows = 0;
	}

	void operator=(const Matrix2D<T> &m)
	{
		cols = m.cols;
		rows = m.rows;
		for (size_t i = 0; i < rows * cols; i++)
		{
			arr[i] = m.as1DArr()[i];
		}
	}

	Matrix2D<T> *operator*(const Matrix2D<T> &m) const
	{
		// for explanation how matrix multiplication work go to:
		// https://www.mathsisfun.com/algebra/matrix-multiplying.html

		if (cols != m.rowCount())
		{
			throw "Matrices can't be multiplied because of sizes";
		}

		Matrix2D<T> *result = new Matrix2D<T>(rows, m.colCount(), true);

		// Calculates values for the new matrix
		for (size_t row = 0; row < m.rowCount(); row++)
		{
			for (size_t col = 0; col < m.colCount(); col++)
			{
				// dot product of two matrices
				T dotProduct = T();
				for (size_t i = 0; i < cols; i++)
				{
					dotProduct += getAt(row, i) * m(i, col);
				}
				result->setAt(row, col, dotProduct);
			}
		}

		return result;
	};

	~Matrix2D() noexcept
	{
		delete[] arr;
	};
};

template <typename T>
std::ostream &operator<<(std::ostream &out, Matrix2D<T> &arr)
{
	for (size_t row = 0; row < arr.rowCount(); row++)
	{
		for (size_t col = 0; col < arr.colCount(); col++)
		{
			out << arr(row, col) << ' ';
		}
		out << std::endl;
	}
	return out;
}
