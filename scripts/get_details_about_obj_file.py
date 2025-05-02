import sys
import numpy as np

def calculate_bounding_sphere(obj_path):
    vertices = []

    with open(obj_path, 'r') as f:
        for line in f:
            if line.startswith('v '):
                parts = line.split()
                vertices.append([float(parts[1]), float(parts[2]), float(parts[3])])

    if not vertices:
        return None

    vertices = np.array(vertices)
    min_coords = np.min(vertices, axis=0)
    max_coords = np.max(vertices, axis=0)

    # Calculate bounding box info
    dimensions = max_coords - min_coords
    center = (max_coords + min_coords) / 2

    # Calculate bounding sphere radius
    radius = np.max(np.linalg.norm(vertices - center, axis=1))

    return {
        'bounding_box': {
            'width': dimensions[0],
            'height': dimensions[1],
            'depth': dimensions[2],
            'min': min_coords.tolist(),
            'max': max_coords.tolist()
        },
        'bounding_sphere': {
            'center': center.tolist(),
            'radius': float(radius)
        }
    }

def print_results(data):
    print("\n=== OBJ Mesh Analysis ===")
    print(f"File: {sys.argv[1]}\n")

    print("Bounding Box Dimensions:")
    print(f"  Width:  {data['bounding_box']['width']:.4f} units")
    print(f"  Height: {data['bounding_box']['height']:.4f} units")
    print(f"  Depth:  {data['bounding_box']['depth']:.4f} units")
    print(f"  Min:    {data['bounding_box']['min']}")
    print(f"  Max:    {data['bounding_box']['max']}\n")

    print("Bounding Sphere:")
    print(f"  Center: {data['bounding_sphere']['center']}")
    print(f"  Radius: {data['bounding_sphere']['radius']:.4f} units\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python get_radius_from_obj_file.py <path/to/model.obj>")
        sys.exit(1)

    try:
        result = calculate_bounding_sphere(sys.argv[1])
        if result:
            print_results(result)
        else:
            print("Error: No vertices found in OBJ file")
    except Exception as e:
        print(f"Error processing file: {e}")