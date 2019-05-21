#pragma once
#include "vector3.hpp"

#include <iostream>

namespace Matrix
{
    template<class T>
    class Multiplication3Dto2D
    {
    public:
        Multiplication3Dto2D() {}
        virtual ~Multiplication3Dto2D() = default;

        Vector3 mult(std::vector<std::vector<T>> vec, int rows, int cols, Vector3 v)
        {
            // Setup
            m_RowsA = rows;
            m_ColsA = cols;

            m_MatrixA = vec;

            vecToMat(v);

            // Math Logic Check
            if (m_ColsA != m_RowsB)
            {
                std::cout << "ColsA must be the same as RowsB!";
            }

            // Resulting Vector
            Vector3 result;

            for (int y = 0; y < m_RowsA; y++)
            {
                for (int x = 0; x < m_ColsB; x++)
                {
                    float sum = 0.0f;

                    for (int k = 0; k < m_ColsA; k++)
                    {
                        sum += m_MatrixA[y][k] * m_MatrixB[k][x];
                    }

                    result.add(sum);
                }
            }
            
            // Return resulting vector
            return result;
        }

    private:
        std::vector<std::vector<T>> m_MatrixA;
        std::vector<std::vector<T>> m_MatrixB;

        int m_RowsA;
        int m_ColsA;

        int m_RowsB;
        int m_ColsB;

        // Convert Vector3 into a 2D vector array
        void vecToMat(Vector3 v)
        {
            m_RowsB = 3;
            m_ColsB  = 1;

            m_MatrixB.clear();
            m_MatrixB.resize(m_RowsB, std::vector<T>(m_ColsB));

            m_MatrixB[0][0] = v.x;
            m_MatrixB[1][0] = v.y;
            m_MatrixB[2][0] = v.z;    
        }
    };
}