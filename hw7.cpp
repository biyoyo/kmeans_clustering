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

    void read_from_file(const string &file_name)
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

    void write_results_to_file(const string &file_name)
    {
        ofstream file(file_name, ios::trunc);

        if (!file)
        {
            cout << "Error opening file for writing" << endl;
        }

        for (const Point &p : points)
        {
            file << p.x << " " << p.y << " " << p.cluster_id << "\n";
        }
    }

    void initialize()
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

        for (Point &p : points)
        {
            p.cluster_id = distro(gen);
        }
    }

    void kmeans_plus_plus_init()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distro(0, points.size() - 1);

        centroids.push_back(points[distro(gen)]);

        for (int i = 0; i < clusters_count - 1; i++)
        {
            vector<double> min_distances;
            for (int j = 0; j < points.size(); j++)
            {
                double min_distance = numeric_limits<double>::infinity();
                for (int k = 0; k < centroids.size(); k++)
                {
                    double distance = point_distance(centroids[k], points[j]);
                    if(distance < min_distance)
                    {
                        min_distance = distance;
                    }
                }
                min_distances.push_back(min_distance);
            }

            double max_distance = -numeric_limits<double>::infinity();
            int next_centroid_index;
            for(int l = 0; l < min_distances.size(); l++)
            {
                if(min_distances[l] > max_distance)
                {
                    max_distance = min_distances[l];
                    next_centroid_index = l;
                }
            }
            centroids.push_back(points[next_centroid_index]);
        }
    }

    void assign_to_class()
    {
        for (Point &p : points)
        {
            double min_distance = numeric_limits<double>::infinity();
            for (int i = 0; i < centroids.size(); i++)
            {
                double distance = point_distance(p, centroids[i]);
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
            if (points_in_cluster > 0)
            {
                centroids[i].x = double(mean_x) / points_in_cluster;
                centroids[i].y = double(mean_y) / points_in_cluster;
            }
        }
    }

    void standard_kmeans()
    {
        initialize();

        int random_restarts = 100;
        for(int i = 0; i < random_restarts; i++)
        {
            vector<int> prev_result = get_current_assignments();
            vector<Point> prev_centroids = centroids;
            double prev_error = within_cluster_distances();

            random_restart();

            int iterations = 100;
            for(int j = 0; j < iterations; j++)
            {
                assign_to_class();
                calculate_means();
            }

            double new_error = within_cluster_distances();
            if(prev_error < new_error)
            {
                set_better_assignments(prev_result, prev_centroids);
            }
        }
    }

    void kmeans_plus_plus()
    {
        kmeans_plus_plus_init();

        int iterations = 300;
        for (int j = 0; j < iterations; j++)
        {
            assign_to_class();
            calculate_means();
        }
    }

    double within_cluster_distances()
    {
        double error = 0;

        for (const Point &p : points)
        {
            error += point_distance(p, centroids[p.cluster_id]);
        }

        return error;
    }

    void random_restart()
    {
        kmeans_plus_plus();
        /*
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distro(0, clusters_count - 1);

        for (Point &p : points)
        {
            p.cluster_id = distro(gen);
        }

        uniform_int_distribution<> distro1(0, points.size() - 1);
        for(Point &p : centroids)
        {
            p = points[distro1(gen)];
        }
        */
    }

    vector<int> get_current_assignments()
    {
        vector<int> result;
        for (const Point &p : points)
        {
            result.push_back(p.cluster_id);
        }
        return result;
    }

    void set_better_assignments(const vector<int> &assignments)
    {
        for (int i = 0; i < assignments.size(); i++)
        {
            points[i].cluster_id = assignments[i];
        }
    }

    struct Point
    {
        double x, y;
        int cluster_id = -1;
    };

    double point_distance(const Point &p1, const Point &p2)
    {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    }

private:
    vector<Point> points;
    int clusters_count;
    vector<Point> centroids;
};

int main()
{
    string file_name;
    int clusters_count;
    cin >> file_name >> clusters_count;
    KMeans k(file_name, clusters_count);
    k.clusterize();
    k.write_results_to_file("results.txt");
    return 0;
}