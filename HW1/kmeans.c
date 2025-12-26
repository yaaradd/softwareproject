#define _GNU_SOURCE
#define EPSILON 0.001
#define MAX_ITER_DEFAULT 400

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ===================== DATA STRUCTURES ===================== */

typedef struct point_coordinates_cell {
    double data;
    struct point_coordinates_cell *next;
} point_coordinates_cell;

typedef struct point_coordinates_list {
    point_coordinates_cell *head;
    point_coordinates_cell *tail;
} point_coordinates_list;

typedef struct points_cell {
    point_coordinates_list *point;
    struct points_cell *next;
} points_cell;

typedef struct points_list {
    points_cell *head;
    points_cell *tail;
    unsigned int length;
} points_list;

typedef struct {
    double *coords;
} centroid;

/* ===================== LINKED LIST HELPERS ===================== */

point_coordinates_list *create_point_coordinates_list() {
    point_coordinates_list *list = malloc(sizeof(point_coordinates_list));
    if (!list) return NULL;
    list->head = list->tail = NULL;
    return list;
}

int add_coordinate(point_coordinates_list *list, double value) {
    point_coordinates_cell *cell = malloc(sizeof(point_coordinates_cell));
    if (!cell) return 0;
    cell->data = value;
    cell->next = NULL;
    if (!list->head) list->head = list->tail = cell;
    else { list->tail->next = cell; list->tail = cell; }
    return 1;
}

points_cell *create_points_cell(point_coordinates_list *point) {
    points_cell *cell = malloc(sizeof(points_cell));
    if (!cell) return NULL;
    cell->point = point;
    cell->next = NULL;
    return cell;
}

int add_point(points_list *plist, point_coordinates_list *point) {
    points_cell *cell = create_points_cell(point);
    if (!cell) return 0;
    if (!plist->head) plist->head = plist->tail = cell;
    else { plist->tail->next = cell; plist->tail = cell; }
    plist->length++;
    return 1;
}

points_list *create_points_list() {
    points_list *plist = malloc(sizeof(points_list));
    if (!plist) return NULL;
    plist->head = plist->tail = NULL;
    plist->length = 0;
    return plist;
}

void free_point_coordinates_list(point_coordinates_list *list) {
    point_coordinates_cell *c, *next;
    if (!list) return;
    c = list->head;
    while (c) {
        next = c->next;
        free(c);
        c = next;
    }
    free(list);
}

void free_points_list(points_list *plist) {
    points_cell *p, *next_p;
    if (!plist) return;
    p = plist->head;
    while (p) {
        free_point_coordinates_list(p->point);
        next_p = p->next;
        free(p);
        p = next_p;
    }
    free(plist);
}

/* ===================== INPUT READING ===================== */

point_coordinates_list *parse_line(const char *line, unsigned int expected_dim) {
    const char *ptr = line;
    unsigned int dim_count = 0;
    int n;
    double value;
    point_coordinates_list *coords = create_point_coordinates_list();
    if (!coords) return NULL;
    while (*ptr) {
        if (sscanf(ptr, "%lf%n", &value, &n) != 1) { free_point_coordinates_list(coords); return NULL; }
        if (!add_coordinate(coords, value)) { free_point_coordinates_list(coords); return NULL; }
        ptr += n;
        dim_count++;
        if (*ptr == ',') ptr++;
        else if (*ptr != '\0') { free_point_coordinates_list(coords); return NULL; }
    }
    if (expected_dim != 0 && dim_count != expected_dim) { free_point_coordinates_list(coords); return NULL; }
    return coords;
}

points_list *read_points(unsigned int *dim) {
    points_list *plist;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    *dim = 0;

    plist = create_points_list();
    if (!plist) return NULL;

    while ((nread = getline(&line, &len, stdin)) != -1) {
        point_coordinates_list *coords;
        if (nread == 1 && line[0] == '\n') break;
        if (line[nread - 1] == '\n') line[nread - 1] = '\0';
        coords = parse_line(line, *dim);
        free(line);
        line = NULL;
        if (!coords) { free_points_list(plist); return NULL; }
        if (plist->length == 0) {
            point_coordinates_cell *c = coords->head;
            unsigned int count = 0;
            while (c) { count++; c = c->next; }
            *dim = count;
        }
        if (!add_point(plist, coords)) { free_point_coordinates_list(coords); free_points_list(plist); return NULL; }
    }
    free(line);
    if (plist->length == 0) { free_points_list(plist); return NULL; }
    return plist;
}

/* ===================== MATRIX CONVERSION ===================== */

double **points_to_matrix(points_list *plist, unsigned int dim) {
    double **matrix;
    points_cell *p;
    unsigned int i, j;

    matrix = malloc(plist->length * sizeof(double*));
    if (!matrix) return NULL;
    p = plist->head;
    i = 0;
    while (p) {
        point_coordinates_cell *c;
        matrix[i] = malloc(dim * sizeof(double));
        if (!matrix[i]) { for (j = 0; j < i; j++) free(matrix[j]); free(matrix); return NULL; }
        c = p->point->head;
        j = 0;
        while (c) {
            matrix[i][j++] = c->data;
            c = c->next;
        }
        p = p->next;
        i++;
    }
    return matrix;
}

void free_matrix(double **matrix, unsigned int n) {
    unsigned int i;
    if (!matrix) return;
    for (i = 0; i < n; i++) free(matrix[i]);
    free(matrix);
}

/* ===================== K-MEANS ===================== */

double distance(double *a, double *b, unsigned int dim) {
    unsigned int i;
    double sum = 0, diff;
    for (i = 0; i < dim; i++) { diff = a[i] - b[i]; sum += diff * diff; }
    return sqrt(sum);
}

centroid *allocate_centroids(unsigned int k, unsigned int dim) {
    centroid *c;
    unsigned int i, j;
    c = malloc(k * sizeof(centroid));
    if (!c) return NULL;
    for (i = 0; i < k; i++) {
        c[i].coords = malloc(dim * sizeof(double));
        if (!c[i].coords) { for (j = 0; j < i; j++) free(c[j].coords); free(c); return NULL; }
    }
    return c;
}

void free_centroids(centroid *c, unsigned int k) {
    unsigned int i;
    if (!c) return;
    for (i = 0; i < k; i++) free(c[i].coords);
    free(c);
}

void copy_centroids(centroid *dest, centroid *src, unsigned int k, unsigned int dim) {
    unsigned int i, j;
    for (i = 0; i < k; i++) for (j = 0; j < dim; j++) dest[i].coords[j] = src[i].coords[j];
}

void assign_labels(double **points, centroid *centroids, unsigned int n, unsigned int k, unsigned int dim, unsigned int *labels) {
    unsigned int i, j, best;
    double best_dist, d;
    for (i = 0; i < n; i++) {
        best = 0;
        best_dist = distance(points[i], centroids[0].coords, dim);
        for (j = 1; j < k; j++) {
            d = distance(points[i], centroids[j].coords, dim);
            if (d < best_dist) { best_dist = d; best = j; }
        }
        labels[i] = best;
    }
}

void update_centroids(double **points, centroid *centroids, unsigned int *labels, unsigned int n, unsigned int k, unsigned int dim) {
    unsigned int i, j, d, count;
    double *sum;
    for (j = 0; j < k; j++) {
        sum = calloc(dim, sizeof(double));
        if (!sum) return;
        count = 0;
        for (i = 0; i < n; i++) {
            if (labels[i] == j) {
                for (d = 0; d < dim; d++) sum[d] += points[i][d];
                count++;
            }
        }
        if (count > 0) for (d = 0; d < dim; d++) centroids[j].coords[d] = sum[d]/count;
        free(sum);
    }
}

double max_centroid_change(centroid *c1, centroid *c2, unsigned int k, unsigned int dim) {
    unsigned int i;
    double max_change = 0, move;
    for (i = 0; i < k; i++) {
        move = distance(c1[i].coords, c2[i].coords, dim);
        if (move > max_change) max_change = move;
    }
    return max_change;
}

void print_centroids(centroid *centroids, unsigned int k, unsigned int dim) {
    unsigned int i, j;
    for (i = 0; i < k; i++) {
        for (j = 0; j < dim; j++) {
            printf("%.4f", centroids[i].coords[j]);
            if (j < dim - 1) printf(",");
        }
        printf("\n");
    }
}

int kmeans(double **points, unsigned int n, unsigned int dim, unsigned int k, unsigned int max_iters) {
    unsigned int i, iter, j;
    unsigned int *labels;
    centroid *centroids, *old_centroids;

    labels = malloc(n * sizeof(unsigned int));
    if (!labels) return 0;
    centroids = allocate_centroids(k, dim);
    old_centroids = allocate_centroids(k, dim);
    if (!centroids || !old_centroids) { free(labels); free_centroids(centroids, k); free_centroids(old_centroids, k); return 0; }

    for (i = 0; i < k; i++)
        for (j = 0; j < dim; j++)
            centroids[i].coords[j] = points[i][j];

    for (iter = 0; iter < max_iters; iter++) {
        assign_labels(points, centroids, n, k, dim, labels);
        copy_centroids(old_centroids, centroids, k, dim);
        update_centroids(points, centroids, labels, n, k, dim);
        if (max_centroid_change(centroids, old_centroids, k, dim) < EPSILON) break;
    }

    print_centroids(centroids, k, dim);
    free(labels);
    free_centroids(centroids, k);
    free_centroids(old_centroids, k);
    return 1;
}

/* ===================== MAIN ===================== */

int is_positive_integer(const char *str) {
    const char *p;
    if (!str || *str == '\0') return 0;
    p = str;
    while (*p) { if (*p < '0' || *p > '9') return 0; p++; }
    return 1;
}

int main(int argc, char *argv[]) {
    points_list *points = NULL;
    double **matrix = NULL;
    unsigned int dim, k, max_iters;

    if (argc < 2 || argc > 3) { fprintf(stderr,"An Error Has Occurred\n"); return 1; }
    if (!is_positive_integer(argv[1])) { fprintf(stderr,"Incorrect number of clusters!\n"); return 1; }
    k = (unsigned int)atoi(argv[1]);

    if (argc == 3) {
        if (!is_positive_integer(argv[2])) { fprintf(stderr,"Incorrect maximum iteration!\n"); return 1; }
        max_iters = (unsigned int)atoi(argv[2]);
    }
    else max_iters = MAX_ITER_DEFAULT;

    points = read_points(&dim);
    if (!points) { fprintf(stderr,"An Error Has Occurred\n"); return 1; }

    if (k <= 1 || k >= points->length) { free_points_list(points); fprintf(stderr,"Incorrect number of clusters!\n"); return 1; }
    if (max_iters <= 1 || max_iters >= 800) { free_points_list(points); fprintf(stderr,"Incorrect maximum iteration!\n"); return 1; }

    matrix = points_to_matrix(points, dim);
    if (!matrix) { free_points_list(points); fprintf(stderr,"An Error Has Occurred\n"); return 1; }

    if (!kmeans(matrix, points->length, dim, k, max_iters)) {
        free_matrix(matrix, points->length);
        free_points_list(points);
        fprintf(stderr,"An Error Has Occurred\n");
        return 1;
    }

    free_matrix(matrix, points->length);
    free_points_list(points);

    return 0;
}