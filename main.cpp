#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>
using namespace std;

const int INF = 1e9;

struct Point {
    int x, y;
    int dist;
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point(int _x, int _y, int _dist) : x(_x), y(_y), dist(_dist) {}
    bool operator>(const Point& other) const {
        return dist > other.dist;
    }
};

vector<vector<int>> generateMap(int rows, int cols) {
    vector<vector<int>> map(rows, vector<int>(cols, 1));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i != 0 && j != 0 && i != rows - 1 && j != cols - 1) {
                map[i][j] = dis(gen);
            }
        }
    }

    map[0][0] = 1;
    map[rows - 1][cols - 1] = 1;

    map[0][cols - 1] = 0;
    map[rows - 1][0] = 0;
    map[0][cols - 2] = 0;
    map[rows - 2][0] = 0;

    return map;
}

vector<Point> BFS(vector<vector<int>>& map, Point start, Point dest) {
    vector<vector<bool>> visited(map.size(), vector<bool>(map[0].size(), false));
    vector<vector<Point>> parent(map.size(), vector<Point>(map[0].size(), Point(-1, -1)));
    queue<Point> q;
    q.push(start);
    visited[start.x][start.y] = true;

    while (!q.empty()) {
        Point current = q.front();
        q.pop();

        if (current.x == dest.x && current.y == dest.y) {
            vector<Point> path;
            while (current.x != -1 && current.y != -1) {
                path.push_back(current);
                current = parent[current.x][current.y];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            if (nx >= 0 && nx < map.size() && ny >= 0 && ny < map[0].size() && map[nx][ny] == 1 && !visited[nx][ny]) {
                visited[nx][ny] = true;
                parent[nx][ny] = current;
                q.push(Point(nx, ny));
            }
        }
    }

    // No se encontró un camino válido
    return vector<Point>();
}

void dfsUtil(vector<vector<int>>& map, Point current, Point dest, vector<vector<bool>>& visited,
             vector<vector<Point>>& parent, vector<Point>& path) {
    visited[current.x][current.y] = true;

    if (current.x == dest.x && current.y == dest.y) {
        while (current.x != -1 && current.y != -1) {
            path.push_back(current);
            current = parent[current.x][current.y];
        }
        reverse(path.begin(), path.end());
        return;
    }

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; ++i) {
        int nx = current.x + dx[i];
        int ny = current.y + dy[i];
        if (nx >= 0 && nx < map.size() && ny >= 0 && ny < map[0].size() && map[nx][ny] == 1 && !visited[nx][ny]) {
            parent[nx][ny] = current;
            dfsUtil(map, Point(nx, ny), dest, visited, parent, path);
        }
    }
}

vector<Point> DFS(vector<vector<int>>& map, Point start, Point dest) {
    vector<vector<bool>> visited(map.size(), vector<bool>(map[0].size(), false));
    vector<vector<Point>> parent(map.size(), vector<Point>(map[0].size(), Point(-1, -1)));
    vector<Point> path;
    dfsUtil(map, start, dest, visited, parent, path);
    return path;
}

vector<Point> Dijkstra(vector<vector<int>>& map, Point start, Point dest) {
    int rows = map.size();
    int cols = map[0].size();

    vector<vector<int>> dist(rows, vector<int>(cols, INF));
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<Point>> parent(rows, vector<Point>(cols, Point(-1, -1, INF)));
    priority_queue<Point, vector<Point>, greater<Point>> pq;

    dist[start.x][start.y] = 0;
    pq.push(start);

    while (!pq.empty()) {
        Point current = pq.top();
        pq.pop();

        if (visited[current.x][current.y]) {
            continue;
        }

        visited[current.x][current.y] = true;

        if (current.x == dest.x && current.y == dest.y) {
            vector<Point> path;
            Point p = dest;
            while (p.x != -1 && p.y != -1) {
                path.push_back(p);
                p = parent[p.x][p.y];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && map[nx][ny] == 1) {
                int newDist = dist[current.x][current.y] + 1;
                if (newDist < dist[nx][ny]) {
                    dist[nx][ny] = newDist;
                    parent[nx][ny] = current;
                    pq.push(Point(nx, ny, newDist));
                }
            }
        }
    }

    return vector<Point>(); // No se encontró un camino válido
}

void displayMap(vector<vector<int>>& map, Point start, Point dest) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (i == start.x && j == start.y) {
                cout << "S ";
            } else if (i == dest.x && j == dest.y) {
                cout << "D ";
            } else {
                cout << (map[i][j] == 0 ? "#" : " ") << " ";
            }
        }
        cout << endl;
    }
}

void displayPath(vector<vector<int>>& map, const vector<Point>& path, Point start, Point dest) {
    vector<vector<int>> tempMap = map;

    for (const auto& point : path) {
        tempMap[point.x][point.y] = -1;
    }

    for (int i = 0; i < tempMap.size(); ++i) {
        for (int j = 0; j < tempMap[0].size(); ++j) {
            if (i == start.x && j == start.y) {
                cout << "S ";
            } else if (i == dest.x && j == dest.y) {
                cout << "D ";
            } else if (tempMap[i][j] == -1) {
                cout << "* ";
            } else {
                cout << (tempMap[i][j] == 0 ? "#" : " ") << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    srand(time(NULL));
    int rows = 10;
    int cols = 15;

    vector<vector<int>> map = generateMap(rows, cols);

    Point start(0, 0);
    Point dest(rows - 1, cols - 1);
    displayMap(map,start,dest);

    vector<Point> pathBFS = BFS(map, start, dest);
    vector<Point> pathDFS = DFS(map, start, dest);
    vector<Point> pathDijkstra = Dijkstra(map, start, dest);

    cout << "Camino encontrado por BFS:" << endl;
    displayPath(map, pathBFS, start, dest);
    cout << endl;

    cout << "Camino encontrado por DFS:" << endl;
    displayPath(map, pathDFS, start, dest);
    cout << endl;

    cout << "Camino encontrado por Dijkstra:" << endl;
    displayPath(map, pathDijkstra, start, dest);
    cout << endl;

    return 0;
}