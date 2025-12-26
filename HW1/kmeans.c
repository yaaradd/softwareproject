#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define EPSILON 0.001
#define MAX_LINE_LENGTH 10000

double euclidean_distance(double *point1, double *point2, int dimension);
void assign_to_clusters(double **datapoints, int N, double **centroids, int K, int dimension, int *assignments);
void update_centroids(double **datapoints, int N, double **centroids, int K, int dimension, int *assignments, int *cluster_sizes);
int has_converged(double **old_centroids, double **new_centroids, int K, int dimension);
void read_input(double ***datapoints, int *N, int *dimension);
void print_centroids(double **centroids, int K, int dimension);
void free_memory(double **array, int rows);
int parse_int_strict(const char *str);

double euclidean_distance(double *point1, double *point2, int dimension) {
    double distance_squared = 0.0;
    int i;
    for (i = 0; i < dimension; i++) {
        double diff = point1[i] - point2[i];
        distance_squared += diff * diff;
    }
    return sqrt(distance_squared);
}

void assign_to_clusters(double **datapoints, int N, double **centroids, int K, int dimension, int *assignments) {
    int i, k;
    double min_distance, distance;
    int closest_cluster;
    for (i = 0; i < N; i++) {
        min_distance = euclidean_distance(datapoints[i], centroids[0], dimension);
        closest_cluster = 0;
        for (k = 1; k < K; k++) {
            distance = euclidean_distance(datapoints[i], centroids[k], dimension);
            if (distance < min_distance) {
                min_distance = distance;
                closest_cluster = k;
            }
        }
        assignments[i] = closest_cluster;
    }
}

void update_centroids(double **datapoints, int N, double **centroids, int K, int dimension, int *assignments, int *cluster_sizes) {
    int i, k, d;
    for (k = 0; k < K; k++) {
        cluster_sizes[k] = 0;
        for (d = 0; d < dimension; d++) {
            centroids[k][d] = 0.0;
        }
    }
    for (i = 0; i < N; i++) {
        int cluster = assignments[i];
        cluster_sizes[cluster]++;
        for (d = 0; d < dimension; d++) {
            centroids[cluster][d] += datapoints[i][d];
        }
    }
    for (k = 0; k < K; k++) {
        if (cluster_sizes[k] > 0) {
            for (d = 0; d < dimension; d++) {
                centroids[k][d] /= cluster_sizes[k];
            }
        }
    }
}

int has_converged(double **old_centroids, double **new_centroids, int K, int dimension) {
    int k;
    double distance;
    for (k = 0; k < K; k++) {
        distance = euclidean_distance(old_centroids[k], new_centroids[k], dimension);
        if (distance >= EPSILON) {
            return 0;
        }
    }
    return 1;
}

int parse_int_strict(const char *str) {
    char *endptr;
    long value;
    if (str == NULL || *str == '\0') {
        return -1;
    }
    value = strtol(str, &endptr, 10);
    if (*endptr != '\0' || endptr == str) {
        return -1;
    }
    return (int)value;
}

void read_input(double ***datapoints, int *N, int *dimension) {
    char line[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];
    char temp[MAX_LINE_LENGTH];
    int capacity, count, dim, i, line_dim, last_was_comma, temp_len;
    double **data;
    double *point;
    double value;
    char *ptr, *start, *end, *endptr;
    size_t len;
    
    capacity = 100;
    count = 0;
    dim = -1;
    
    data = (double **)malloc(capacity * sizeof(double *));
    if (data == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    
    while (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }
        
        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        strcpy(line_copy, line);
        
        ptr = line_copy;
        line_dim = 0;
        last_was_comma = 1;
        
        while (*ptr != '\0') {
            if (*ptr == ',') {
                if (last_was_comma) {
                    printf("An Error Has Occurred\n");
                    free_memory(data, count);
                    exit(1);
                }
                last_was_comma = 1;
                line_dim++;
            } else if (!isspace(*ptr)) {
                last_was_comma = 0;
            }
            ptr++;
        }
        
        if (last_was_comma && line_dim > 0) {
            printf("An Error Has Occurred\n");
            free_memory(data, count);
            exit(1);
        }
        
        line_dim++;
        
        if (dim == -1) {
            dim = line_dim;
        } else if (line_dim != dim) {
            printf("An Error Has Occurred\n");
            free_memory(data, count);
            exit(1);
        }
        
        point = (double *)malloc(dim * sizeof(double));
        if (point == NULL) {
            printf("An Error Has Occurred\n");
            free_memory(data, count);
            exit(1);
        }
        
        ptr = line;
        i = 0;
        
        while (*ptr != '\0' && i < dim) {
            start = ptr;
            
            while (*ptr != '\0' && *ptr != ',') {
                ptr++;
            }
            
            end = ptr;
            
            while (start < end && isspace(*start)) {
                start++;
            }
            while (end > start && isspace(*(end - 1))) {
                end--;
            }
            
            if (start == end) {
                printf("An Error Has Occurred\n");
                free(point);
                free_memory(data, count);
                exit(1);
            }
            
            temp_len = end - start;
            
            if (temp_len >= MAX_LINE_LENGTH) {
                printf("An Error Has Occurred\n");
                free(point);
                free_memory(data, count);
                exit(1);
            }
            
            strncpy(temp, start, temp_len);
            temp[temp_len] = '\0';
            
            value = strtod(temp, &endptr);
            
            if (*endptr != '\0') {
                printf("An Error Has Occurred\n");
                free(point);
                free_memory(data, count);
                exit(1);
            }
            
            point[i] = value;
            i++;
            
            if (*ptr == ',') {
                ptr++;
            }
        }
        
        if (count >= capacity) {
            capacity *= 2;
            data = (double **)realloc(data, capacity * sizeof(double *));
            if (data == NULL) {
                printf("An Error Has Occurred\n");
                free(point);
                exit(1);
            }
        }
        
        data[count] = point;
        count++;
    }
    
    *datapoints = data;
    *N = count;
    *dimension = (dim == -1) ? 0 : dim;
}

void print_centroids(double **centroids, int K, int dimension) {
    int k, d;
    for (k = 0; k < K; k++) {
        for (d = 0; d < dimension; d++) {
            printf("%.4f", centroids[k][d]);
            if (d < dimension - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}

void free_memory(double **array, int rows) {
    int i;
    if (array != NULL) {
        for (i = 0; i < rows; i++) {
            if (array[i] != NULL) {
                free(array[i]);
            }
        }
        free(array);
    }
}

int main(int argc, char *argv[]) {
    int K, max_iter, N, dimension, iter, i, d;
    double **datapoints, **centroids, **old_centroids;
    int *assignments, *cluster_sizes;
    
    max_iter = 400;
    
    if (argc < 2 || argc > 3) {
        printf("An Error Has Occurred\n");
        return 1;
    }
    
    K = parse_int_strict(argv[1]);
    if (K == -1) {
        printf("Incorrect number of clusters!\n");
        return 1;
    }
    
    if (argc == 3) {
        max_iter = parse_int_strict(argv[2]);
        if (max_iter == -1) {
            printf("Incorrect maximum iteration!\n");
            return 1;
        }
    }
    
    read_input(&datapoints, &N, &dimension);
    
    if (N == 0) {
        printf("An Error Has Occurred\n");
        return 1;
    }
    
    if (K <= 1 || K >= N) {
        printf("Incorrect number of clusters!\n");
        free_memory(datapoints, N);
        return 1;
    }
    
    if (max_iter <= 1 || max_iter >= 800) {
        printf("Incorrect maximum iteration!\n");
        free_memory(datapoints, N);
        return 1;
    }
    
    centroids = (double **)malloc(K * sizeof(double *));
    old_centroids = (double **)malloc(K * sizeof(double *));
    if (centroids == NULL || old_centroids == NULL) {
        printf("An Error Has Occurred\n");
        free_memory(datapoints, N);
        return 1;
    }
    
    for (i = 0; i < K; i++) {
        centroids[i] = (double *)malloc(dimension * sizeof(double));
        old_centroids[i] = (double *)malloc(dimension * sizeof(double));
        if (centroids[i] == NULL || old_centroids[i] == NULL) {
            printf("An Error Has Occurred\n");
            free_memory(datapoints, N);
            free_memory(centroids, K);
            free_memory(old_centroids, K);
            return 1;
        }
    }
    
    assignments = (int *)malloc(N * sizeof(int));
    cluster_sizes = (int *)malloc(K * sizeof(int));
    if (assignments == NULL || cluster_sizes == NULL) {
        printf("An Error Has Occurred\n");
        free_memory(datapoints, N);
        free_memory(centroids, K);
        free_memory(old_centroids, K);
        return 1;
    }
    
    for (i = 0; i < K; i++) {
        for (d = 0; d < dimension; d++) {
            centroids[i][d] = datapoints[i][d];
        }
    }
    
    for (iter = 0; iter < max_iter; iter++) {
        for (i = 0; i < K; i++) {
            for (d = 0; d < dimension; d++) {
                old_centroids[i][d] = centroids[i][d];
            }
        }
        
        assign_to_clusters(datapoints, N, centroids, K, dimension, assignments);
        update_centroids(datapoints, N, centroids, K, dimension, assignments, cluster_sizes);
        
        if (has_converged(old_centroids, centroids, K, dimension)) {
            break;
        }
    }
    
    print_centroids(centroids, K, dimension);
    
    free_memory(datapoints, N);
    free_memory(centroids, K);
    free_memory(old_centroids, K);
    free(assignments);
    free(cluster_sizes);
    
    return 0;
}
