#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <limits>

using namespace std;

class KMeans
{
public:
    KMeans(const string &file_name, int clusters_count)
        : clusters_count(clusters_count)
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

        for (;;)
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

    void initialize_centroids()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distro(0, clusters_count - 1);

        for (int i = 0; i < clusters_count; i++)
        {
            int random_index = distro(gen);
            centroids.push_back(points[random_index]);
            points[random_index].cluster_id = i;
        }
    }

    void assign_to_class()
    {
        for (Point &p : points)
        {
            double min_distance = numeric_limits<int>::max();
            for (int i = 0; i < centroids.size(); i++)
            {
                double distance = (centroids[i].x - p.x) * (centroids[i].x - p.x) + (centroids[i].y - p.y) * (centroids[i].y - p.y);
                if (distance < min_distance)
                {
                    p.cluster_id = i;
                    min_distance = distance;
                }
            }
        }
    }

    void calculate_means()
    {
        for (int i = 0; i < clusters_count; i++)
        {
            double mean_x = 0, mean_y = 0;
            int points_in_cluster = 0;
            for (const Point &p : points)
            {
                if (p.cluster_id == i)
                {
                    mean_x += p.x;
                    mean_y += p.y;
                    points_in_cluster++;
                }
            }
            if(points_in_cluster > 0)
            {
                centroids[i].x = double(mean_x) / points_in_cluster;
                centroids[i].y = double(mean_y) / points_in_cluster;
            }
        }
    }

    void clusterize()
    {
        initialize_centroids();
        int iterations = 0;
        do
        {
            assign_to_class();
            calculate_means();
            iterations++;
        } while (iterations != 300);
    }

    struct Point
    {
        double x, y;
        int cluster_id = -1;
    };

private:
    vector<Point> points;
    int clusters_count;
    vector<Point> centroids;
};

int main()
{
    string file_name;
    int clusters_count;
    //cin >> file_name >> clusters_count;
    KMeans k("normal.txt", 4);
    //KMeans k(file_name, clusters_count);
    k.clusterize();
    //k.assign_to_class();
    return 0;
}