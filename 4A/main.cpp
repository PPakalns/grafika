#include <stdio.h>
#include <exception>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <random>

#include <opencv2/opencv.hpp>
#include "core/Utility.h"

template <typename T>
struct Vec4{
    T val[4];

    static Vec4<T> getPos(const T& x, const T& y, const T& z)
    {
        return {x, y, z, 1};
    }

    static Vec4<T> getDirection(const T& x, const T& y, const T& z)
    {
        return {x, y, z, 0};
    }

    Vec4<T> operator-() const
    {
        return {-val[0], -val[1], -val[2], val[3]};
    }

    T length() const
    {
        return sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
    }

    Vec4<T> normalize() const
    {
        T l = length();
        return {
            val[0] / l,
            val[1] / l,
            val[2] / l,
            val[3]
        };
    }

    Vec4<T> cross(const Vec4<T>& r) const
    {
        return {
            val[1] * r.val[2] - val[2] * r.val[1],
            val[2] * r.val[0] - val[0] * r.val[2],
            val[0] * r.val[1] - val[1] * r.val[0],
            0
        };
    }

    Vec4<T> operator-(const Vec4<T>& r) const
    {
        // Assume - both points with the same W
        return {val[0] - r.val[0], val[1] - r.val[1], val[2] - r.val[2], val[3]};
    }

    Vec4<T> toScreen() const
    {
        return {val[0] / val[3], val[1] / val[3], val[2] / val[3], 1};
    }
};

template <typename T>
struct Mat4{
    T val[4][4];

    Mat4<T> operator*(const Mat4<T>& lhs) const
    {
        Mat4<T> tmp;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                tmp.val[i][j] = 0;
                for (int k = 0; k < 4; k++)
                {
                    tmp.val[i][j] += val[i][k] * lhs.val[k][j];
                }
            }
        }
        return tmp;
    }

    Vec4<T> operator*(const Vec4<T>& lhs) const
    {
        Vec4<T> tmp;
        for (int i = 0; i < 4; i++)
        {
            tmp.val[i] = 0;
            for (int j = 0; j < 4; j++)
            {
                tmp.val[i] += val[i][j] * lhs.val[j];
            }
        }
        return tmp;
    }

    static Mat4<T> getRoationX(double rad)
    {
        return {
            1, 0, 0, 0,
            0, cos(rad), -sin(rad), 0,
            0, sin(rad), cos(rad), 0,
            0, 0, 0, 1,
        };
    }

    static Mat4<T> getRoationY(double rad)
    {
        return {
            cos(rad), 0, sin(rad), 0,
            0, 1, 0, 0,
            -sin(rad), 0, cos(rad), 0,
            0, 0, 0, 1,
        };
    }

    static Mat4<T> getRoationZ(double rad)
    {
        return {
            cos(rad), -sin(rad), 0, 0,
            sin(rad), cos(rad), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    static Mat4<T> getScale(T x, T y, T z)
    {
        return {
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1,
        };
    }

    static Mat4<T> getUnit()
    {
        return getScale(1, 1, 1);
    }

    static Mat4<T> getTranslate(T x, T y, T z)
    {
        return {
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1,
        };
    }

    static Mat4<T> getTranslate(const Vec4<T>& vec)
    {
        return getTranslate(vec.val[0], vec.val[1], vec.val[2]);
    }

    static Mat4<T> getXYShearing(T coef)
    {
        return {
            1, coef, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
    }

    static Mat4<T> getPerspectiveProjection(T coef)
    {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, -1 / coef, 0
        };
    }
};

template<typename T>
Mat4<T> CameraLookAt(const Vec4<T>& cameraPos, const Vec4<T>& lookTo, const Vec4<T>& up)
{
    auto forward = (cameraPos - lookTo).normalize();
    auto right = up.normalize().cross(forward);
    auto rup = forward.cross(right);
    return Mat4<T>{
        right.val[0], right.val[1], right.val[2], 0,
        rup.val[0], rup.val[1], rup.val[2], 0,
        forward.val[0], forward.val[1], forward.val[2], 0,
        0, 0, 0, 1,
    } * Mat4<double>::getTranslate(-cameraPos);
}

const double PI = acos(-1.0L);

std::vector<std::tuple<Vec4<double>, Vec4<double>>> DodecahedronEdges()
{
    // Calculate all dodecahedron vertices
    std::vector<Vec4<double>> point;
    double fi = (1 + sqrt(5)) / 2;
    for (int i = -1; i < 2; i+=2)
    {
        for (int j = -1; j < 2; j+=2)
        {
            point.push_back({0, i * fi, j * (1/fi), 1});
            point.push_back({i * (1/fi), 0, j * fi, 1});
            point.push_back({i * fi, j * (1/fi), 0, 1});
            for (int k = -1; k < 2; k+=2)
            {
                point.push_back({(double)i, (double)j, (double)k, 1});
            }
        }
    }

    std::vector<std::tuple<Vec4<double>, Vec4<double>>> edges;
    const double expected = sqrt(5) - 1;

    for (size_t i = 0; i < point.size(); i++)
    {
        for (size_t j = i + 1; j < point.size(); j++)
        {
            double dist = (point[i] - point[j]).length();
            if (abs(expected - dist) < 0.0000001)
            {
                edges.emplace_back(point[i], point[j]);
            }
        }
    }

    return edges;
}

cv::Mat DrawImage(const std::vector<std::tuple<Vec4<double>, Vec4<double>>>& edges,
                  Mat4<double> mat,
                  bool perspective = false)
{
    const int SW = 800;
    if (perspective)
    {
        mat = Mat4<double>::getPerspectiveProjection(1) * mat;
    }

    cv::Mat screen = cv::Mat::zeros({SW, SW}, CV_8U);

    auto toPoint = [](Vec4<double> vec) {
        return cv::Point((0.5 + vec.val[0]) * SW, (0.5 + vec.val[1]) * SW);
    };

    for (const auto& e : edges)
    {
        Vec4<double> e1, e2;
        std::tie(e1, e2) = e;
        auto a1 = toPoint((mat * e1).toScreen());
        auto a2 = toPoint((mat * e2).toScreen());
        cv::line(screen, a1, a2, 255);
    }
    return screen;
}

int safe_main(int, char**)
{
    std::random_device device;
    std::mt19937 generator(device());
    // Choosing random parameters
    auto degDist = std::uniform_real_distribution<>(0.0L, PI);
    auto scaleDist = std::uniform_real_distribution<>(0.2L, 0.4L);
    auto numberDist = std::uniform_real_distribution<>(-1L, 1L);
    auto shearingDist = std::uniform_real_distribution<>(0.5, 1.5);

    using V4D = Vec4<double>;
    auto initialPos = V4D::getPos(2, 2, 2);
    auto cameraPos = V4D::getPos(-1, 1, 1);

    auto camera = CameraLookAt(cameraPos, initialPos, V4D{0, 1, 0, 0});

    auto resProj = camera
                   * Mat4<double>::getTranslate(numberDist(generator), numberDist(generator), numberDist(generator))
                   * Mat4<double>::getTranslate(initialPos)
                   * Mat4<double>::getRoationX(degDist(generator))
                   * Mat4<double>::getRoationY(degDist(generator))
                   * Mat4<double>::getRoationZ(degDist(generator))
                   * Mat4<double>::getXYShearing(shearingDist(generator))
                   * Mat4<double>::getRoationX(degDist(generator))
                   * Mat4<double>::getRoationY(degDist(generator))
                   * Mat4<double>::getRoationZ(degDist(generator))
                   * Mat4<double>::getScale(scaleDist(generator), scaleDist(generator), scaleDist(generator))
                   ;

    auto dEdges = DodecahedronEdges();
    core::ImageWindow("Orthogonal projection", DrawImage(dEdges, resProj));
    core::ImageWindow("Perspective projection", DrawImage(dEdges, resProj, true));

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
