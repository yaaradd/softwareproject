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
            # This shouldn't happen with valid input, but handle it
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
    
    # Initialize centroids as first k datapoints
    centroids = [datapoints[i][:] for i in range(k)]
    
    for iteration in range(max_iter):
        # Assign datapoints to clusters
        clusters = assign_to_clusters(datapoints, centroids)
        
        # Update centroids
        new_centroids = update_centroids(clusters, dimension)
        
        # Check for convergence
        if has_converged(centroids, new_centroids):
            centroids = new_centroids
            break
        
        centroids = new_centroids
    
    return centroids

def read_input():
    """Read datapoints from stdin."""
    datapoints = []
    
    for line in sys.stdin:
        line = line.strip()
        if line:  # Skip empty lines
            values = line.split(',')
            point = [float(val) for val in values]
            datapoints.append(point)
    
    return datapoints

def print_centroids(centroids):
    """Print centroids formatted to 4 decimal places."""
    for centroid in centroids:
        formatted = ','.join(['%.4f' % val for val in centroid])
        print(formatted)

def main():
    try:
        # Parse command line arguments
        if len(sys.argv) < 2 or len(sys.argv) > 3:
            print("An Error Has Occurred")
            sys.exit(1)
        
        # Get K
        try:
            k = int(sys.argv[1])
        except ValueError:
            print("Incorrect number of clusters!")
            sys.exit(1)
        
        # Get max_iter (default 400)
        max_iter = 400
        if len(sys.argv) == 3:
            try:
                max_iter = int(sys.argv[2])
            except ValueError:
                print("Incorrect maximum iteration!")
                sys.exit(1)
        
        # Read datapoints
        datapoints = read_input()
        N = len(datapoints)
        
        # Validate K
        if k <= 1 or k >= N:
            print("Incorrect number of clusters!")
            sys.exit(1)
        
        # Validate max_iter
        if max_iter <= 1 or max_iter >= 800:
            print("Incorrect maximum iteration!")
            sys.exit(1)
        
        # Run K-means
        centroids = kmeans(datapoints, k, max_iter)
        
        # Print results
        print_centroids(centroids)
        
    except Exception:
        print("An Error Has Occurred")
        sys.exit(1)

if __name__ == "__main__":
    main()
