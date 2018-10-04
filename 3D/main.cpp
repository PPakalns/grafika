#include <stdio.h>
#include <exception>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include "core/Utility.h"

using ll = long long;

struct Point{
    int x, y;
};

struct Segment{
    Point a, b;

    Segment(const Point& ain, const Point& bin)
        : a(ain)
        , b(bin)
    {
        if (a.x > b.x || (a.x == b.x && a.y > b.y))
        {
            std::swap(a, b);
        }
        assert(b.x >= a.x && (a.x != b.x || a.y < b.y));
    }
};

struct FastFloat {
    static const int FAST_FLOAT_PREC = 16;
    ll val;
    explicit FastFloat() {};
    explicit FastFloat(int ival)
        : val(static_cast<ll>(ival) << FAST_FLOAT_PREC)
    {
    }

    FastFloat operator/(const FastFloat& rhs) const
    {
        FastFloat tmp;
        tmp.val = (val << FAST_FLOAT_PREC) / rhs.val;
        return tmp;
    }

    bool operator<(const FastFloat& rhs) const
    {
        return val < rhs.val;
    }

    FastFloat& operator+=(const FastFloat& rhs)
    {
        val += rhs.val;
        return *this;
    }

    FastFloat operator*(const FastFloat& rhs)
    {
        FastFloat tmp;
        tmp.val = (val * rhs.val) >> FAST_FLOAT_PREC;
        return tmp;
    }

    FastFloat operator*(int rhs)
    {
        FastFloat tmp;
        tmp.val = (val * rhs);
        return tmp;
    }

    int getRoundedPositive()
    {
        return (val + (1 << (FAST_FLOAT_PREC - 1))) >> FAST_FLOAT_PREC;
    }
};

struct HorizontalSegment{
    FastFloat y, a;
    int x, endx;
    HorizontalSegment(Segment& rhs, int xPos)
    {
        x = rhs.a.x;
        y = FastFloat(rhs.a.y);
        FastFloat dx(rhs.b.x - rhs.a.x);
        FastFloat dy(rhs.b.y - rhs.a.y);
        a = dy / dx;
        endx = rhs.b.x;

        if (x < xPos)
        {
            y += a * (xPos - x);
            x = xPos;
        }
    }

    bool operator<(const HorizontalSegment& rhs) const
    {
        return y < rhs.y;
    }

    void advance(int xPos)
    {
        if (x >= xPos)
            return;
        // x + 1 == xPos
        y += a;
        ++x;
    }
};

std::vector<Segment> ReadSegments(const std::string& path, int& H, int& W)
{
    std::ifstream input(path);
    if (input.is_open() == false)
    {
        throw GrafikaException("Couldn't open provided file: " + path);
    }

    std::vector<Segment> segments;
    std::vector<Point> points;
    int n;
    input >> W >> H;
    input >> n;
    std::cout << "Poligons ar " << n << " virsotnēm" << std::endl;
    if (n < 3)
    {
        throw GrafikaException("Poligonam jāsastāv no vismaz trīs malām");
    }
    points.reserve(n); segments.reserve(n);
    for (int i = 0; i < n; i++)
    {
        int x, y;
        input >> x >> y;
        points.push_back({x, y});
    }

    segments.emplace_back(*points.begin(), *points.rbegin());
    for (int i = 1; i < n; i++)
    {
        segments.emplace_back(points[i - 1], points[i]);
    }

    return segments;
}

bool xcompare(const Segment& a, const Segment& b)
{
    return a.a.x < b.a.x;
}

void DrawLine(cv::Mat& mat, int x, int y1, int y2, int H)
{
    y1 = std::max(y1, 0);
    y2 = std::min(y2, H - 1);

    while (y1 <= y2)
    {
        mat.at<unsigned char>(y1, x) = 255;
        ++y1;
    }
}

void DrawPolygon(std::vector<Segment> seg, cv::Mat& mat)
{
    std::sort(seg.begin(), seg.end(), xcompare);

    int H = mat.rows;
    int W = mat.cols;

    size_t fidx = 0;
    int xPos = 0;

    std::vector<HorizontalSegment> iseg;

    xPos = std::max(seg[0].a.x, 0);

    while (xPos < W && (fidx == 0 || iseg.size() > 0))
    {
        while (fidx < seg.size() && seg[fidx].a.x <= xPos)
        {
            if (seg[fidx].a.x != seg[fidx].b.x)
            {
                iseg.emplace_back(seg[fidx], xPos);
            }
            fidx++;
        }

        iseg.erase(std::remove_if(iseg.begin(), iseg.end(),
                                  [=](const HorizontalSegment& hseg)
                                  { return hseg.endx == xPos; }),
                   iseg.end());

        for (auto& t : iseg)
        {
            t.advance(xPos);
        }

        // Bubble sort
        for (size_t sz = iseg.size(); sz > 0; sz--)
        {
            bool cont = false;
            for (size_t i = 1; i < sz; i++)
            {
                if (iseg[i] < iseg[i - 1])
                {
                    std::swap(iseg[i], iseg[i - 1]);
                    cont = true;
                }
            }
            if (cont == false)
                break;
        }

        for (size_t i = 1; i < iseg.size(); i+=2)
        {
            DrawLine(mat, xPos, iseg[i-1].y.getRoundedPositive(), iseg[i].y.getRoundedPositive(), H);
        }

        xPos++;
    }
}

int safe_main(int argc, char** argv)
{
    if (argc != 2)
    {
        throw GrafikaException("No input file provided! Usage: ./progr <text file containing description>");
    }

    int W, H;
    std::vector<Segment> seg = ReadSegments(argv[1], W, H);
    cv::Mat mat = cv::Mat::zeros(H, W, CV_8U);
    DrawPolygon(seg, mat);

    core::ImageWindow window("Polygon", mat);

    return 0;
}

int main(int argc, char** argv)
{
    return core::CatchExceptions(safe_main, argc, argv);
}
