//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс, вычисляющий кратчайшее расстояние между заданными точками.
/// Использует алгоритм поиска в ширину.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "findtheway.h"
#include <queue>

FindTheWay::FindTheWay()
{}

// вычисление пути (передача копии вектора, вместо указателя для потокобезопасности)
void FindTheWay::findTheWay(const std::vector<std::vector<int>> neighbors, int start, int finish, PathType type)
{
    stop = false;

    int maxWay = neighbors.size();
    std::vector<int> dist(maxWay, -1);
    std::vector<int> parent(maxWay, -1);
    std::queue<int> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        if (stop) return;

        int node = q.front();
        q.pop();
        for (auto neighbor : neighbors[node]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                parent[neighbor] = node;
                q.push(neighbor);
            }
        }
    }

    std::vector<int> path;
    if (dist[finish] != -1) {
        for (int v = finish; v != -1; v = parent[v]) {
            if (stop) return;

            path.push_back(v);
        }
        std::reverse(path.begin(),path.end());
    }

    emit foundPath(path, type);
}
