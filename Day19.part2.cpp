#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <numeric>


struct Vector3
{
    int x, y, z;

    int manhattan() const	{ return std::abs(x) + std::abs(y) + std::abs(z); }
 
    Vector3 operator-(const Vector3 &other) const
    {
        return Vector3{x-other.x, y-other.y, z-other.z};
    }

    Vector3 operator+(const Vector3 &other) const
    {
        return Vector3{x+other.x, y+other.y, z+other.z};
    }

    bool operator==(const Vector3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator<(const Vector3 &other) const
    {
        return x < other.x ||
               (x == other.x && y < other.y) ||
               (x == other.x && y == other.y && z < other.z);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v)
    {
        os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
        return os;
    }
};



struct Matrix3x3
{
    int m[3][3];

    friend Vector3 operator*(const Matrix3x3 &mat, const Vector3 &vec)
    {
        return Vector3{vec.x*mat.m[0][0] + vec.y*mat.m[0][1] + vec.z*mat.m[0][2],
                       vec.x*mat.m[1][0] + vec.y*mat.m[1][1] + vec.z*mat.m[1][2],
                       vec.x*mat.m[2][0] + vec.y*mat.m[2][1] + vec.z*mat.m[2][2]};
    }
};



struct Scanner
{
    // Scanner name
    std::string name;

    // Scanner position
    Vector3 pos{0, 0, 0};

    // The source list of beacon data
    std::vector<Vector3> beacons;
};



std::vector<Scanner> readScanners()
{
    std::vector<Scanner> scanners;

    while (true)
    {
        Scanner scanner;

        // Read the scanner name line
        std::string line;
        if (!std::getline(std::cin, line))
            break;
        scanner.name = line.substr(3, line.size()-6);

        // Read beacon data (no validation of input comma separators)
        char c;
        Vector3 beacon;
        while (std::cin >> beacon.x >> c >> beacon.y >> c >> beacon.z)
        {
            scanner.beacons.emplace_back(beacon);
        }

        scanners.emplace_back(scanner);

        // Skip whitespace
        std::cin.clear();
        std::cin >> std::ws;
    }

    return scanners;
}


// There are 24 possible orientations. Simplest way to visualize is to consider
// a dice. There are four orientations possible where each number on the dice is
// at the top, giving 6 x 4 orientations in total.

const Matrix3x3 orientations[] = {
    Matrix3x3{  1,  0,  0,    0,  1,  0,    0,  0,  1 },
    Matrix3x3{  0, -1,  0,    1,  0,  0,    0,  0,  1 },
    Matrix3x3{ -1,  0,  0,    0, -1,  0,    0,  0,  1 },
    Matrix3x3{  0,  1,  0,   -1,  0,  0,    0,  0,  1 },

    Matrix3x3{  0,  0, -1,    0,  1,  0,    1,  0,  0 },
    Matrix3x3{  0, -1,  0,    0,  0, -1,    1,  0,  0 },
    Matrix3x3{  0,  0,  1,    0, -1,  0,    1,  0,  0 },
    Matrix3x3{  0,  1,  0,    0,  0,  1,    1,  0,  0 },

    Matrix3x3{ -1,  0,  0,    0,  1,  0,    0,  0, -1 },
    Matrix3x3{  0, -1,  0,   -1,  0,  0,    0,  0, -1 },
    Matrix3x3{  1,  0,  0,    0, -1,  0,    0,  0, -1 },
    Matrix3x3{  0,  1,  0,    1,  0,  0,    0,  0, -1 },

    Matrix3x3{  0,  0,  1,    0,  1,  0,   -1,  0,  0 },
    Matrix3x3{  0, -1,  0,    0,  0,  1,   -1,  0,  0 },
    Matrix3x3{  0,  0, -1,    0, -1,  0,   -1,  0,  0 },
    Matrix3x3{  0,  1,  0,    0,  0, -1,   -1,  0,  0 },

    Matrix3x3{  1,  0,  0,    0,  0, -1,    0,  1,  0 },
    Matrix3x3{  0,  0,  1,    1,  0,  0,    0,  1,  0 },
    Matrix3x3{ -1,  0,  0,    0,  0,  1,    0,  1,  0 },
    Matrix3x3{  0,  0, -1,   -1,  0,  0,    0,  1,  0 },

    Matrix3x3{  1,  0,  0,    0,  0,  1,    0, -1,  0 },
    Matrix3x3{  0,  0, -1,    1,  0,  0,    0, -1,  0 },
    Matrix3x3{ -1,  0,  0,    0,  0, -1,    0, -1,  0 },
    Matrix3x3{  0,  0,  1,   -1,  0,  0,    0, -1,  0 }
};


const std::tuple<bool, Scanner> resolveScanner(const Scanner &scanner1, const Scanner &scanner2)
{
    // Check all orientations
    for (const auto &orientation : orientations)
    {
        // Reorient all beacons in the test Scanner
        Scanner testScanner = scanner2;
        for (auto &beacon : testScanner.beacons)
            beacon = orientation*beacon;

        // For all beacons in the origin scanner, create an offset to each beacon
        // in the test scanner and see if we can obtain a match
        for (const auto &beacon1 : scanner1.beacons)
        {
            for (const auto &beacon2 : testScanner.beacons)
            {
                Vector3 offset = beacon2 - beacon1;

                int matches{0};
                for (const auto &b1 : scanner1.beacons)
                {
                    for (const auto &b2 : testScanner.beacons)
                    {
                        if (b2 - b1 - offset == Vector3{0, 0, 0})
                        {
                            if (++matches >= 12)
                            {
                                testScanner.pos = scanner1.pos - offset;
                                return std::make_tuple(true, testScanner);
                            }
                        }
                    }
                }
            }
        }
    }

    return std::make_tuple(false, Scanner{});
}



int main(int, char **)
{
    // Read all scanners from the input
    std::vector<Scanner> scanners = readScanners();

    // Make the first scanner the ground truth, for orientation and origin. Add to a list
    // of resolved scanners, and remove from the list of unresolved scanners
    std::vector<Scanner> resolvedScanners;
    std::vector<Scanner> unresolvedScanners = scanners;
    resolvedScanners.reserve(scanners.size());
    resolvedScanners.push_back(unresolvedScanners.front());
    unresolvedScanners.erase(unresolvedScanners.begin());

    // Setup list of resolved beacons. Initially, these will be from the first scanner,
    // making the baseline orientation and position
    std::set<Vector3> resolvedBeacons;
    for (const auto &beacon : resolvedScanners.front().beacons)
        resolvedBeacons.insert(beacon);

    // While there are still unresolved scanners, loop over each resolved scanner, and try
    // to match it to each unresolved scanner
    int lastResolved = 0;
    while (!unresolvedScanners.empty())
    {
        bool foundScanner = false;

        const auto &scanner = resolvedScanners[lastResolved++];

        for (auto it = unresolvedScanners.begin(); it != unresolvedScanners.end(); )
        {
            const auto &[success, resolvedScanner] = resolveScanner(scanner, *it);
            if (success)
            {
                for (const auto &beacon : resolvedScanner.beacons)
                {
                    resolvedBeacons.insert(beacon + resolvedScanner.pos);
                }

                resolvedScanners.emplace_back(resolvedScanner);
                it = unresolvedScanners.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    std::cout << "Total beacons: " << resolvedBeacons.size() << "\n";

    int dist{0};
    for (int i = 0; i < resolvedScanners.size(); ++i)
    {
        for (int j = i+1; j < resolvedScanners.size(); ++j)
        {
            int manhattanDistance = (resolvedScanners[i].pos - resolvedScanners[j].pos).manhattan();
            dist = std::max(manhattanDistance, dist);
        }
    }
    std::cout << "Maximum distance = " << dist << "\n";


    return 0;
}
