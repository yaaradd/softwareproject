import sys

def euclidean_distance(point1, point2):
    """Calculate Euclidean distance between two points."""
    distance_squared = 0.0
    for i in range(len(point1)):
        distance_squared += (point1[i] - point2[i]) ** 2
    return distance_squared ** 0.5

def assign_to_clusters(datapoints, centroids):
    """Assign each datapoint to the nearest centroid."""
    clusters = [[] for _ in range(len(centroids))]
    
    for point in datapoints:
        min_distance = float('inf')
        closest_cluster = 0
        
        for k in range(len(centroids)):
            distance = euclidean_distance(point, centroids[k])
            if distance < min_distance:
                min_distance = distance
                closest_cluster = k
        
        clusters[closest_cluster].append(point)
    
    return clusters

def update_centroids(clusters, dimension):
    """Calculate new centroids as the mean of cluster members."""
    new_centroids = []
    
    for cluster in clusters:
        if len(cluster) == 0:
            new_centroids.append([0.0] * dimension)
        else:
            centroid = [0.0] * dimension
            for point in cluster:
                for i in range(dimension):
                    centroid[i] += point[i]
            
            for i in range(dimension):
                centroid[i] /= len(cluster)
            
            new_centroids.append(centroid)
    
    return new_centroids

def has_converged(old_centroids, new_centroids, epsilon=0.001):
    """Check if all centroids have moved less than epsilon."""
    for i in range(len(old_centroids)):
        distance = euclidean_distance(old_centroids[i], new_centroids[i])
        if distance >= epsilon:
            return False
    return True

def kmeans(datapoints, k, max_iter):
    """Perform K-means clustering."""
    dimension = len(datapoints[0])
    
    centroids = [datapoints[i][:] for i in range(k)]
    
    for iteration in range(max_iter):
        clusters = assign_to_clusters(datapoints, centroids)
        new_centroids = update_centroids(clusters, dimension)
        
        if has_converged(centroids, new_centroids):
            centroids = new_centroids
            break
        
        centroids = new_centroids
    
    return centroids

def read_input():
    """Read and validate datapoints from stdin."""
    datapoints = []
    dimension = None
    
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        
        try:
            values = line.split(',')
            point = []
            
            for val_str in values:
                val_str = val_str.strip()
                if not val_str:  # Empty value
                    print("An Error Has Occurred")
                    sys.exit(1)
                
                try:
                    point.append(float(val_str))
                except ValueError:
                    print("An Error Has Occurred")
                    sys.exit(1)
            
            # Check dimension consistency
            if dimension is None:
                dimension = len(point)
            elif len(point) != dimension:
                print("An Error Has Occurred")
                sys.exit(1)
            
            datapoints.append(point)
        except Exception:
            print("An Error Has Occurred")
            sys.exit(1)
    
    return datapoints

def print_centroids(centroids):
    """Print centroids formatted to 4 decimal places."""
    for centroid in centroids:
        formatted = ','.join(['%.4f' % val for val in centroid])
        print(formatted)

def parse_int_strict(s):
    """Strictly parse integer - rejects 2.5, 2x, etc."""
    if not s:
        return None
    
    try:
        num = int(s)
        if str(num) == s:
            return num
        else:
            return None
    except ValueError:
        return None

def main():
    try:
        if len(sys.argv) < 2 or len(sys.argv) > 3:
            print("An Error Has Occurred")
            sys.exit(1)
        
        k = parse_int_strict(sys.argv[1])
        if k is None:
            print("Incorrect number of clusters!")
            sys.exit(1)
        
        max_iter = 400
        if len(sys.argv) == 3:
            max_iter = parse_int_strict(sys.argv[2])
            if max_iter is None:
                print("Incorrect maximum iteration!")
                sys.exit(1)
        
        datapoints = read_input()
        N = len(datapoints)
        
        if N == 0:
            print("An Error Has Occurred")
            sys.exit(1)
        
        if k <= 1 or k >= N:
            print("Incorrect number of clusters!")
            sys.exit(1)
        
        if max_iter <= 1 or max_iter >= 800:
            print("Incorrect maximum iteration!")
            sys.exit(1)
        
        centroids = kmeans(datapoints, k, max_iter)
        print_centroids(centroids)
        
    except Exception:
        print("An Error Has Occurred")
        sys.exit(1)

if __name__ == "__main__":
    main()
