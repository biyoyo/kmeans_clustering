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

    void assign_to_class()
    {
        for (Point &p : points)
        {
            double min_distance = numeric_limits<long int>::max();
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
            if (points_in_cluster > 0)
            {
                centroids[i].x = double(mean_x) / points_in_cluster;
                centroids[i].y = double(mean_y) / points_in_cluster;
            }
        }
    }

    void clusterize()
    {
        initialize();
        int random_restarts = 0;
        do
        {
            vector<int> prev_result = get_current_assignments();
            vector<double> prev_grade = within_cluster_distances();
            random_restart();

            int iterations = 0;
            do
            {
                assign_to_class();
                calculate_means();
                iterations++;
            } while (iterations != 300);

            vector<int> new_result = get_current_assignments();
            vector<double> new_grade = within_cluster_distances();

            if (compare_clusters(prev_grade, new_grade) == true)
            {
                set_better_assignments(prev_result);
            }

            random_restarts++;

        } while (random_restarts != 5);
    }

    vector<double> within_cluster_distances()
    {
        vector<double> within_distances(clusters_count, 0);

        for (const Point &p : points)
        {
            double distance = (p.x - centroids[p.cluster_id].x) * (p.x - centroids[p.cluster_id].x) +
                              (p.y - centroids[p.cluster_id].y) * (p.y - centroids[p.cluster_id].y);
            within_distances[p.cluster_id] += distance;
        }
        return within_distances;
    }

    void random_restart()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distro(0, clusters_count - 1);

        for (Point &p : points)
        {
            p.cluster_id = distro(gen);
        }
    }

    //result with more thight clusters wins
    static bool compare_clusters(const vector<double> &result1, const vector<double> &result2)
    {
        int successes1 = 0, successes2 = 0;

        for (int i = 0; i < result1.size(); i++)
        {
            if (result1[i] <= result2[i])
            {
                successes1++;
            }
            else
            {
                successes2++;
            }
        }
        return successes1 > successes2;
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
    //KMeans k(file_name, clusters_count);
    KMeans k("normal.txt", 4);
    k.clusterize();
    k.write_results_to_file("results.txt");
    return 0;
}