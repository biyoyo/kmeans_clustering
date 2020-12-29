#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class KMeans
{
    public:
    KMeans(const string &file_name, int clusters_count)
    :clusters_count(clusters_count)
    {
        read_from_file(file_name);
    }
    void read_from_file(string file_name)
    {
        ifstream file(file_name);

        if (!file)
        {
            cout << "Error opening file" << endl;
        }

        for(;;)
        {
            double x, y;
            file >> x >> y;

            if (file.eof())
            {
                break;
            }

            if (!file)
            {
                cout << "Error reading file" << endl;
            }

            Point point;
            point.x = x;
            point.y = y;
            points.push_back(point);
        }
        file.close();
    }

    struct Point
    {
        double x, y;
    };
    
    private:
    vector<Point> points;
    int clusters_count;
};

int main()
{
    string file_name;
    int clusters_count;
    cin >> file_name >> clusters_count;
    KMeans(file_name, clusters_count);
    return 0;
}