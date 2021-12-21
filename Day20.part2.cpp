#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>


class Image2D
{
public:
    Image2D(int width, int height);

    static Image2D read(std::istream &in);

    int width() const		{ return width_; }
    int height() const		{ return height_; }

    void setEdge(int val)	{ edge_ = val; }

    int get(int x, int y) const;

    Image2D process(const std::string &algorithm);

    friend std::ostream &operator<<(std::ostream &os, const Image2D &im);

private:
    int width_, height_;
    int edge_{0};
    std::vector<int> data_;
};


Image2D::Image2D(int width, int height)
: width_(width), height_(height)
{
    data_.resize(width*height);
}


int Image2D::get(int x, int y) const
{
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
        return data_[x+y*width_];
    return edge_;
}


Image2D Image2D::read(std::istream &in)
{
    std::vector<std::string> sourceData;

    std::string data;
    while (in >> data)
        sourceData.emplace_back(data);

    int sourceWidth = sourceData[0].size();
    int sourceHeight = sourceData.size();
    Image2D image(sourceWidth+10, sourceHeight+10);
    for (int y = 0; y < sourceHeight; ++y)
    {
        for (int x = 0; x < sourceWidth; ++x)
        {
            image.data_[x+5 + (y+5)*image.width_] = (sourceData[y][x] == '#');
        }
    }

    return image;
}


Image2D Image2D::process(const std::string &algorithm)
{
    // As the algorithm is applying a 3x3 filter over the image, it's conceivable that
    // the image will grow by 1 pixel on each side
    Image2D result(width()+2, height()+2);

    // Set the edge colour based on the current edge colour and how the algorithm would
    // change that colour
    result.setEdge(algorithm[edge_ * ((1 << 9)-1)]);

    // Apply the algorithm over each pixel
    for (int y = 0; y < result.height(); ++y)
    {
        for (int x = 0; x < result.width(); ++x)
        {
            int idx = (get(x-2, y-2) << 8) +
                      (get(x-1, y-2) << 7) +
                      (get(x  , y-2) << 6) +
                      (get(x-2, y-1) << 5) +
                      (get(x-1, y-1) << 4) +
                      (get(x  , y-1) << 3) +
                      (get(x-2, y  ) << 2) +
                      (get(x-1, y  ) << 1) +
                      (get(x  , y  ) << 0);
            result.data_[x + y*result.width()] = (algorithm[idx] == '#');
        }
    }

    // Ideally, we should take the image and remove borders to avoid increasing the
    // image size too much
    return result;
}


std::ostream &operator<<(std::ostream &os, const Image2D &im)
{
    for (int y = 0; y < im.height(); ++y)
    {
        for (int x = 0; x < im.width(); ++x)
        {
            os << (im.get(x, y) ? "#" : ".");
        }
        os << "\n";
    }
    return os;
}


int main(int, char **)
{
    // Read the algorithm
    std::string algorithm;
    std::cin >> algorithm;
    if (algorithm.size() != 1 << 9)
        throw std::runtime_error("Invalid algorithm\n");

    // Read the image
    Image2D image = Image2D::read(std::cin);

    // Process twice
    for (int i = 0; i < 50; i++)
    {
        image = image.process(algorithm);
    }

    // Count lit pixels
    int litPixels{0};
    for (int y = 0; y < image.height(); ++y)
        for (int x = 0; x < image.width(); ++x)
            litPixels += image.get(x, y);

    std::cout << "Lit pixels after 50 iterations: " << litPixels << "\n";

    return 0;
}

