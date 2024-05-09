#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 1000
#define MAX_COLS 2
#define PI 3.14159265359
#define FIXED_BIT 10

/*============================== Define Struct ===================================*/
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point p1;
    Point p2;
} PointPair;

typedef struct KDNode {
    Point point;
    struct KDNode* left;
    struct KDNode* right;
} KDNode;

typedef struct {
    double distance;
    double indices;
} Result;

typedef struct {
    double rotation[2][2];
    double translation[2];
} TransformationMatrix;

/*============================== Convert to Binary from Dec  ===================================*/
void decToBinary(int n, FILE *file) 
{ 
    // array to store binary number 
    int binaryNum[20]; 
  
    // counter for binary array 
    int i = 0; 
    while (n > 0  || i < 20) { 
  
        // storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    } 

    for (int j = 19; j >= 0; j--) 
        fprintf(file, "%d", (j >= i) ? 0 : binaryNum[j]); 
    
    //fprintf(file, " "); 
  
    // printing binary array in reverse order 
    //for (int j = 15; j >= 0; j--) 
    //    printf("%d", (j >= i) ? 0 : binaryNum[j]); 
    
    //printf("\n"); 
} 

/*============================== Convert to Fixed from Floating  ===================================*/
unsigned int float2fix(float n)
{
    unsigned int int_part = 0, frac_part = 0;
    int i;
    float t;

    int_part = (int)floor(n) << FIXED_BIT;
    n -= (int)floor(n);

    t = 0.5;
    for (i = 0; i < FIXED_BIT; i++) {
        if ((n - t) >= 0) {
            n -= t;
            frac_part += (1 << (FIXED_BIT - 1 - i));
        }
        t = t /2;
    }
    //printf("int part: %d\n",int_part);
    //printf("frac part:%d\n",frac_part);
    //decToBinary(int_part+frac_part);

    return int_part + frac_part;
}


/*============================== Read CSV File ===================================*/
int readCSV(const char *filename, Point reference_points[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file.");
        return -1; // Return -1 on failure
    }

    int row = 0, col = 0;
    char line[1024];

     while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int col = 0;

        while (token != NULL && col < MAX_COLS) {
            if (col == 0) {
                reference_points[row].x = atof(token);
            } else if (col == 1) {
                reference_points[row].y = atof(token);
            }

            col++;
            token = strtok(NULL, ",");
        }

        row++;
    }

    fclose(file);
    return row; // Return the number of rows read from the CSV
}


/*============================== Euclidean Distance Function ===================================*/
double euclidean_distance(double point1[], double point2[], int n) {
    double distance = 0.0;

    for (int i = 0; i < n; i++) {
        distance += pow(point1[i] - point2[i], 2); 

    };

    return sqrt(distance);

};


/*============================== Point Based Matching Function ===================================*/
void point_based_matching(PointPair point_pairs[], int n, double *rot_angle, double *translation_x, double *translation_y) {
    double x_mean = 0, y_mean = 0, xp_mean = 0, yp_mean = 0;

    if (n == 0) {
        *rot_angle = 0;
        *translation_x = 0;
        *translation_y = 0;
        return;
    };

    for (int i = 0; i < n; i++) {
        x_mean += point_pairs[i].p1.x;
        y_mean += point_pairs[i].p1.y;
        xp_mean += point_pairs[i].p2.x;
        yp_mean += point_pairs[i].p2.y;
    };

    x_mean /= n;
    y_mean /= n;
    xp_mean /= n;
    yp_mean /= n;
    double s_x_xp = 0, s_y_yp = 0, s_x_yp = 0, s_y_xp = 0;

    for (int i = 0; i < n; i++) {
        s_x_xp += (point_pairs[i].p1.x - x_mean)*(point_pairs[i].p2.x - xp_mean);
        s_y_yp += (point_pairs[i].p1.y - y_mean)*(point_pairs[i].p2.y - yp_mean);
        s_x_yp += (point_pairs[i].p1.x - x_mean)*(point_pairs[i].p2.y - yp_mean);
        s_y_xp += (point_pairs[i].p1.y - y_mean)*(point_pairs[i].p2.x - xp_mean);
    };

    *rot_angle = atan2(s_x_yp - s_y_xp, s_x_xp + s_y_yp);
    *translation_x = xp_mean - (x_mean * cos(*rot_angle) - y_mean * sin(*rot_angle));
    *translation_y = yp_mean - (x_mean * sin(*rot_angle) + y_mean * cos(*rot_angle));

}

/*============================== 2D Distance Function ===================================*/
double distanceSquared(Point p1, Point p2) {
    return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
}
int nodeCount = 0;
KDNode nodes[MAX_ROWS];

/*==============================  KD-tree Function ===================================*/
KDNode* insert(KDNode* root, Point point, int depth) {
    if (root == NULL) {
        if (nodeCount < MAX_ROWS) {
            nodes[nodeCount].point = point;
            nodes[nodeCount].left = NULL;
            nodes[nodeCount].right = NULL;
            return &nodes[nodeCount++];
        } else {
            return NULL; // Maximum capacity reached
        }
    }

    int cd = depth % 2;
    

    if (cd == 0) {
        if (point.x < root->point.x) {
            root->left = insert(root->left, point, depth + 1);
        } else {
            root->right = insert(root->right, point, depth + 1);
        }
    } else {
        if (point.y < root->point.y) {
            root->left = insert(root->left, point, depth + 1);
        } else {
            root->right = insert(root->right, point, depth + 1);
        }
    }

    return root;
}


/*==============================  Nearest Neighbor Function ===================================*/
void nearestNeighborSearch(KDNode* root, Point point, KDNode** nearest, double* minDistance, int depth) {
    if (root == NULL) {
        return;
    }

    double dist = distanceSquared(root->point, point);

    if (dist < *minDistance) {
        *minDistance = dist;
        *nearest = root;
    }
    printf("minDistance: %f\n", *minDistance);
    printf("depth: %d\n", depth);
    printf("source_x: %f\n", point.x);
    printf("source_y: %f\n", point.y);
    printf("reference_x: %f\n", root->point.x);
    printf("reference_y: %f\n", root->point.y);

    int cd = depth % 2;

    if (cd == 0) {
        if (point.x < root->point.x) {
            nearestNeighborSearch(root->left, point, nearest, minDistance, depth + 1);
            if (point.x + sqrt(*minDistance) >= root->point.x) {
                nearestNeighborSearch(root->right, point, nearest, minDistance, depth + 1);
            }
        } else {
            nearestNeighborSearch(root->right, point, nearest, minDistance, depth + 1);
            if (point.x - sqrt(*minDistance) <= root->point.x) {
                nearestNeighborSearch(root->left, point, nearest, minDistance, depth + 1);
            }
        }
    } else {
        if (point.y < root->point.y) {
            nearestNeighborSearch(root->left, point, nearest, minDistance, depth + 1);
            if (point.y + sqrt(*minDistance) >= root->point.y) {
                nearestNeighborSearch(root->right, point, nearest, minDistance, depth + 1);
            }
        } else {
            nearestNeighborSearch(root->right, point, nearest, minDistance, depth + 1);
            if (point.y - sqrt(*minDistance) <= root->point.y) {
                nearestNeighborSearch(root->left, point, nearest, minDistance, depth + 1);
            }
        }
    }
}

/*============================== matrixMultiplication Function ===================================*/
void matrixMultiplication(Point points[], int num_points, double rot[][2], Point aligned_points[]) {
    for (int j = 0; j < num_points; ++j) {
        aligned_points[j].x = points[j].x * rot[0][0] + points[j].y * rot[1][0];
        aligned_points[j].y = points[j].x * rot[0][1] + points[j].y * rot[1][1];
    }
}

/*============================== ICP Function ===================================*/
void icp (Point *reference_points, Point *source_points, int num_reference, int num_source, TransformationMatrix *transformation_history, int *history_count) {
    int max_iterations=100;
    double  distance_threshold=0.3;
    double convergence_translation_threshold = 1e-3;
    double convergence_rotation_threshold=1e-4;
    int point_pairs_threshold=10;
    bool verbose = false;
    double rotation_angle, translation_x, translation_y;


    for (int i = 0; i < max_iterations; i++) {
        if (verbose) {
            printf("------ iteration%d ------", i);
        };
        //printf("------ iteration ------");
        Result result[MAX_ROWS];
        PointPair closest_point_pairs[MAX_ROWS];
        int num_closest_point_pairs = 0;

        KDNode* root = NULL;
        for (int k = 0; k < num_reference; ++k) {
            root = insert(root, reference_points[k], 0);
        }

        for (int k = 0; k < num_source; ++k) {
            KDNode* nearest = NULL;
            double minDistance = INFINITY;
            nearestNeighborSearch(root, source_points[k], &nearest, &minDistance, 0);
            if (nearest != NULL) {
                result[k].distance = sqrt(minDistance);
                result[k].indices = 0;
            }

            if (nearest != NULL && result[k].distance < distance_threshold) {
                closest_point_pairs[num_closest_point_pairs].p1 = source_points[k];
                closest_point_pairs[num_closest_point_pairs].p2 = nearest->point;
                num_closest_point_pairs++;
            }
            printf("=========================================================\n");
        }

        // For the test
        /*
        for (int k = 0; k < num_closest_point_pairs; ++k) {
            printf("Closest points: (%.4f, %.4f) and (%.4f, %.4f)\n",
               closest_point_pairs[k].p1.x, closest_point_pairs[k].p1.y,
               closest_point_pairs[k].p2.x, closest_point_pairs[k].p2.y);
        }
        printf("======================================================\n");
        */

        if (verbose) {
            printf("number of pairs found: %d\n", num_closest_point_pairs);
        };
        if (num_closest_point_pairs < point_pairs_threshold) {
            if (verbose) {
                printf("No better solution can be found (very few point pairs)!");
            };
            //break; // In actual algorithm, make it Uncomment it
        };
    
        point_based_matching(closest_point_pairs, num_closest_point_pairs, &rotation_angle, &translation_x, &translation_y);
        
        if (!isnan(rotation_angle)) {
            if (verbose) {
                printf("Rotation: %f degress\n", rotation_angle* 180.0 / PI);
                printf("Translation: %f, %f\n", translation_x, translation_y);
            };

            if (isnan(rotation_angle) || isnan(translation_x) || isnan(translation_y)) {
                if (verbose) {
                    printf("No better solution can be found!\n");
                    break;
                };
            }
        };


        double c = cos(rotation_angle);
        double s = sin(rotation_angle);
        
        double rot[2][2] = {{c, -s},{s, c}};
        double rot_transpose[2][2] = {{c, s},{-s, c}};
        Point aligned_points[MAX_ROWS];

        matrixMultiplication(source_points, num_source, rot_transpose, aligned_points);

        for (int k = 0; k < num_source; k++){
            aligned_points[k].x += translation_x;
            aligned_points[k].y += translation_y;
            source_points[k].x = aligned_points[k].x;
            source_points[k].y = aligned_points[k].y;
        };

        if (*history_count < MAX_ROWS) {
            memcpy(transformation_history[*history_count].rotation, rot, sizeof(rot));
            transformation_history[*history_count].translation[0] = translation_x;
            transformation_history[*history_count].translation[1] = translation_y;
            *history_count += 1;
        } else {
            printf("Maximum history size reached.\n");
        }
        
        if ((abs(rotation_angle) < convergence_rotation_threshold) && (abs(translation_x) < convergence_translation_threshold) && (abs(translation_y) < convergence_translation_threshold)) {
            if (verbose) {
                printf("Converged!\n");
                break;
            };
        };

    };

};


/*============================== Main Function ===================================*/
int main() {
    Point reference_points[MAX_ROWS];
    Point source_points[MAX_ROWS];
    Point source_points2[MAX_ROWS];
    TransformationMatrix transformation_history[MAX_ROWS];
    int history_count = 0;
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();
    //const char *filename = "reference_points.csv";
    const char *filename = "reference_points_test.csv";
    int num_reference_points = readCSV(filename, reference_points);

    if (num_reference_points == -1) {
        printf("Error reading file.");
        return 1;
    }

    //filename = "source_points.csv"; 
    filename = "source_points_test.csv"; 
    int num_source_points = readCSV(filename, source_points);

    if (num_source_points == -1) {
        printf("Error reading file.");
        return 1;
    }

    //printf("%d %d \n", num_reference_points,num_source_points);
    icp(reference_points, source_points, num_reference_points, num_source_points, transformation_history, &history_count);
    printf("=================================================\n");
    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("CPU time used: %.20f seconds\n", cpu_time_used);

    /* For the output test */
    
    //for (int i = 0; i < num_source_points; i++) {
    //    printf("(%.10f, %.10f)\n", source_points[i].x, source_points[i].y);
    //};
    
    /* For the history test */
    /*
    for (int i = 0; i < history_count; ++i) {
        
        printf("Transformation %d:\n", i + 1);
        printf("Rotation Matrix:\n");
            for (int row = 0; row < 2; ++row) {
                for (int col = 0; col < 2; ++col) {
                      printf("%.15f\t", transformation_history[i].rotation[row][col]);
                }
                printf("\n");
            }
        printf("Translation Vector: [%.15f, %.15f]\n\n", 
           transformation_history[i].translation[0], 
           transformation_history[i].translation[1]);
    }
    */

    
    FILE *output_file = fopen("fixed_point_binary_test5.txt", "w");
    if (output_file == NULL) {
        printf("Error opening output file.");
        return 1;
    }

    for (int i = 0; i < num_reference_points; i++) {
        unsigned int fixed_ref_x = float2fix(reference_points[i].x);
        unsigned int fixed_ref_y = float2fix(reference_points[i].y);
        //unsigned short int fixed_src_x = float2fix(source_points[i].x);
        //unsigned short int fixed_src_y = float2fix(source_points[i].y);
        decToBinary(fixed_ref_x, output_file);
        decToBinary(fixed_ref_y, output_file);
        //decToBinary(fixed_src_x, output_file);
        //decToBinary(fixed_src_y, output_file);
        fprintf(output_file, "\n");
    }

    fclose(output_file);
    

    return 0;
};
