/*
    AUTHOR: miha53cevic
    DATE: 30.8.2019.
    Github: https://github.com/miha53cevic
    Resources used - https://www.mathsisfun.com/algebra/matrix-multiplying.html
*/

#pragma once
#include <iostream>

namespace Matrix
{
    template<class T>
    class mat3x3
    {
    public:
        mat3x3()
        {
            // By default set the matrix to be an IDENTITY matrix
            m_mat[0][0] = 1; m_mat[0][1] = 0; m_mat[0][2] = 0;
            m_mat[1][0] = 0; m_mat[1][1] = 1; m_mat[1][2] = 0;
            m_mat[2][0] = 0; m_mat[2][1] = 0; m_mat[2][2] = 1;
        }

        // Initialize matrix with 1D array in constructor
        mat3x3(T mat[])
        {
            m_mat[0][0] = mat[0]; m_mat[0][1] = mat[1]; m_mat[0][2] = mat[2];
            m_mat[1][0] = mat[3]; m_mat[1][1] = mat[4]; m_mat[1][2] = mat[5];
            m_mat[2][0] = mat[6]; m_mat[2][1] = mat[7]; m_mat[2][2] = mat[8];
        }

        // Initialize matrix with 1D array in function
        void initialize(T mat[])
        {
            m_mat[0][0] = mat[0]; m_mat[0][1] = mat[1]; m_mat[0][2] = mat[2];
            m_mat[1][0] = mat[3]; m_mat[1][1] = mat[4]; m_mat[1][2] = mat[5];
            m_mat[2][0] = mat[6]; m_mat[2][1] = mat[7]; m_mat[2][2] = mat[8];
        }

        // Print out matrix values
        void dump()
        {
            for (int y = 0; y < 3; y++)
            {
                std::cout << "{ ";

                for (int x = 0; x < 3; x++)
                {
                    std::cout << m_mat[y][x];

                    if (x != 2)
                        std::cout << ", ";
                }

                std::cout << " }" << std::endl;
            }
        }

        // Get a value in the matrix at a specified index
        T at(int rows, int cols)
        {
            if (rows < 0 || rows >= 3 || cols < 0 || cols >= 3)
            {
                std::cout << "Out of matrix bounds!\n";
                return 0;
            }

            return m_mat[rows][cols];
        }

        // Matrix multiplication
        mat3x3<T> mult(mat3x3<T>& mat)
        {
            T result[9];
            result[0] = this->at(0, 0) * mat.at(0, 0) + this->at(0, 1) * mat.at(1, 0) + this->at(0, 2) * mat.at(2, 0);
            result[1] = this->at(0, 0) * mat.at(0, 1) + this->at(0, 1) * mat.at(1, 1) + this->at(0, 2) * mat.at(2, 1);
            result[2] = this->at(0, 0) * mat.at(0, 2) + this->at(0, 1) * mat.at(1, 2) + this->at(0, 2) * mat.at(2, 2);

            result[3] = this->at(1, 0) * mat.at(0, 0) + this->at(1, 1) * mat.at(1, 0) + this->at(1, 2) * mat.at(2, 0);
            result[4] = this->at(1, 0) * mat.at(0, 1) + this->at(1, 1) * mat.at(1, 1) + this->at(1, 2) * mat.at(2, 1);
            result[5] = this->at(1, 0) * mat.at(0, 2) + this->at(1, 1) * mat.at(1, 2) + this->at(1, 2) * mat.at(2, 2);

            result[6] = this->at(2, 0) * mat.at(0, 0) + this->at(2, 1) * mat.at(1, 0) + this->at(2, 2) * mat.at(2, 0);
            result[7] = this->at(2, 0) * mat.at(0, 1) + this->at(2, 1) * mat.at(1, 1) + this->at(2, 2) * mat.at(2, 1);
            result[8] = this->at(2, 0) * mat.at(0, 2) + this->at(2, 1) * mat.at(1, 2) + this->at(2, 2) * mat.at(2, 2);

            return mat3x3<T>(result);
        }

        // Operator Overloads
        mat3x3<T> operator* (mat3x3<T>& mat)
        {
            return mult(mat);
        }

        void operator*= (mat3x3<T>& mat)
        {
            mat3x3<T> temp = mult(mat);

            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    m_mat[y][x] = temp.at(y, x);
                }
            }
        }

    private:
        // 3x3 matrix container    
        T m_mat[3][3];
    };



    // Will be used for 2D affine transformation matrix calculations
    template<class T>
    class mat3x1
    {
    public:
        mat3x1()
        {
            // By default set the matrix to be filled with 0
            m_mat[0][0] = 0; m_mat[1][0] = 0; m_mat[2][0] = 0;
        }

        // Initialize matrix with 1D array in constructor
        mat3x1(T mat[])
        {
            m_mat[0][0] = mat[0]; m_mat[1][0] = mat[1]; m_mat[2][0] = mat[2];
        }

        // Initialize matrix with paramaters
        mat3x1(T x, T y, T z)
        {
            m_mat[0][0] = x; m_mat[1][0] = y; m_mat[2][0] = z;
        }

        // Initialize matrix with 1D array in function
        void initialize(T mat[])
        {
            m_mat[0][0] = mat[0]; m_mat[1][0] = mat[1]; m_mat[2][0] = mat[2];
        }

        // Initialize matrix with paramaters
        void initialize(T x, T y, T z)
        {
            m_mat[0][0] = x; m_mat[1][0] = y; m_mat[2][0] = z;
        }

        // Print out matrix values
        void dump()
        {
            for (int y = 0; y < 3; y++)
            {
                std::cout << "{ ";

                for (int x = 0; x < 1; x++)
                {
                    std::cout << m_mat[y][x];
                }

                std::cout << " }" << std::endl;
            }
        }

        // Get a value in the matrix at a specified index
        T at(int rows, int cols)
        {
            if (rows < 0 || rows >= 3 || cols < 0 || cols >= 1)
            {
                std::cout << "Out of matrix bounds!\n";
                return 0;
            }

            return m_mat[rows][cols];
        }

        // Matrix multiplication with only a 3x3
        mat3x1<T> mult(mat3x3<T>& mat)
        {
            T result[3];
            result[0] = this->at(0, 0) * mat.at(0, 0) + this->at(1, 0) * mat.at(0, 1) + this->at(2, 0) * mat.at(0, 2);
            result[1] = this->at(0, 0) * mat.at(1, 0) + this->at(1, 0) * mat.at(1, 1) + this->at(2, 0) * mat.at(1, 2);
            result[2] = this->at(0, 0) * mat.at(2, 0) + this->at(1, 0) * mat.at(2, 1) + this->at(2, 0) * mat.at(2, 2);

            return mat3x1<T>(result);
        }

        // Operator Overloads
        mat3x1<T> operator* (mat3x3<T>& mat)
        {
            return mult(mat);
        }

        void operator*= (mat3x3<T>& mat)
        {
            mat3x1<T> temp = mult(mat);

            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 1; x++)
                {
                    m_mat[y][x] = temp.at(y, x);
                }
            }
        }

    private:
        T m_mat[3][1];
    };
}