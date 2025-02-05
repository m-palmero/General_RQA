# include "common.h"
# include "recurrence_functions.h"

void embed_time_series(double **embedded_time_series, double *time_series, int n, int dim, int tau) 
{
    for (int i = 0; i < n - (dim - 1) * tau; i++) {
        for (int j = 0; j < dim; j++) {
            embedded_time_series[i][j] = time_series[i + j * tau];
        }
    }
}

void manhattan_distance(double **embedded_time_series, int n, int dim, double **D) 
{
    double temp;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            temp = 0.0;
            for (int k = 0; k < dim; k++) {
                temp += fabs(embedded_time_series[i][k] - embedded_time_series[j][k]);
            }
            D[i][j] = temp;
            D[j][i] = temp;
        }
    }
}

void euclidean_distance(double **embedded_time_series, int n, int dim, double **D) 
{
    double temp;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            temp = 0.0;
            for (int k = 0; k < dim; k++) {
                temp += pow(embedded_time_series[i][k] - embedded_time_series[j][k], 2);
            }
            D[i][j] = sqrt(temp);
            D[j][i] = sqrt(temp);
        }
    }
}

void supremum_distance(double **embedded_time_series, int n, int dim, double **D) 
{
    double max, temp;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            max = fabs(embedded_time_series[i][0] - embedded_time_series[j][0]);
            for (int k = 1; k < dim; k++) {
                temp = fabs(embedded_time_series[i][k] - embedded_time_series[j][k]);
                if (temp > max) {
                    max = temp;
                }
            }
            D[i][j] = max;
            D[j][i] = max;
        }
    }
}

void modulated_distance(double **embedded_time_series, int n, int dim, double **D) 
{
    double diff;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            diff = 0.0;
            for (int k = 0; k < dim; k++) {
                double temp = fmod(fabs(embedded_time_series[i][k] - embedded_time_series[j][k]), 2 * M_PI);
                if (temp > M_PI) {
                    temp = 2 * M_PI - temp;
                }
                diff += temp * temp;
            }
            D[i][j] = sqrt(diff);
            D[j][i] = sqrt(diff);
        }
    }
}

void calculate_distance_matrix(double **embedded_time_series, int n, int dim, double **D, const char *norm) 
{
    if (strcmp(norm, "manhattan") == 0) manhattan_distance(embedded_time_series, n, dim, D);
    else if (strcmp(norm, "euclidean") == 0) euclidean_distance(embedded_time_series, n, dim, D);
    else if (strcmp(norm, "supremum") == 0) supremum_distance(embedded_time_series, n, dim, D);
    else if (strcmp(norm, "modulated") == 0) modulated_distance(embedded_time_series, n, dim, D);
    else
    {
        fprintf(stderr, "Unknown norm type: %s\n", norm);
        exit(1);
    }
}

void compute_recurrence_matrix(double **D, int n, int **R, double threshold) 
{
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
                if (D[i][j] < threshold) R[i][j] = 1;
                else R[i][j] = 0;
        }
    }
}

double compute_recurrence_rate(int **R, int n) 
{
    int count_recurrent_points = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && R[i][j] == 1) {
                count_recurrent_points++;
            }
        }
    }

    return (double) count_recurrent_points / (double) (n * n);
}

double adjust_threshold_via_recurrence_rate(double **D, int n, int **R, double desired_percentage, int *status) 
{
    if (n <= 0) {
        *status = -1; // Invalid size
        return -1.0;
    }

    double low = 0.0;
    double high = 0.0;

    // Compute the maximum distance in the distance matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (D[i][j] > high) {
                high = D[i][j];
            }
        }
    }

    double mid, recurrence_rate;
    double closest_diff = 1e10; // Start with a large value
    double best_mid = 0.0;

    *status = 1; // Default to closest achievable

    while (high - low > 1e-10) { // Precision threshold
        mid = (low + high) / 2.0;
        compute_recurrence_matrix(D, n, R, mid);
        recurrence_rate = compute_recurrence_rate(R, n) * 100;

        double diff = fabs(recurrence_rate - desired_percentage);
        if (diff < closest_diff) {
            closest_diff = diff;
            best_mid = mid;
        }

        if (diff < 1e-2) { // If within acceptable precision
            *status = 0; // Exact match
            break;
        }

        if (recurrence_rate < desired_percentage) {
            low = mid;
        } else {
            high = mid;
        }
    }

    return *status == 0 ? mid : best_mid;
}

int* calculate_diagonal_frequency_distribution(int** R, int n) 
{
    int i, j, k, diagonal_line_length;
    int *diagonal_frequency_distribution = (int*)malloc((n + 1) * sizeof(int));
    
    if (diagonal_frequency_distribution == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    for(i = 0; i < n + 1; i++)
        diagonal_frequency_distribution[i] = 0;

    for(i = n - 1; i > -1; i--) {
        diagonal_line_length = 0;
        for (j = 0; j < n - i; j++) {
            if (R[i + j][j] == 1) {
                diagonal_line_length += 1;
                if (j == (n - i - 1))
                    diagonal_frequency_distribution[diagonal_line_length] += 1;
            } else {
                if (diagonal_line_length != 0) {
                    diagonal_frequency_distribution[diagonal_line_length] += 1;
                    diagonal_line_length = 0;
                }
            }
        }
    }

    for (k = 1; k < n; k++) {
        diagonal_line_length = 0;
        for (i = 0; i < n - k; i++) {
            j = i + k;
            if (R[i][j] == 1) {
                diagonal_line_length += 1;
                if (j == (n - 1))
                    diagonal_frequency_distribution[diagonal_line_length] += 1;
            } else {
                if (diagonal_line_length != 0) {
                    diagonal_frequency_distribution[diagonal_line_length] += 1;
                    diagonal_line_length = 0;
                }
            }
        }
    }

    return diagonal_frequency_distribution;
}

int* calculate_vertical_frequency_distribution(int** R, int n)
{
    int i, j;
    int *vertical_frequency_distribution = (int*)malloc((n + 1) * sizeof(int));
    
    if (vertical_frequency_distribution == NULL) {
        // Handle memory allocation failure
        return NULL;
    }

    for (i = 0; i < n + 1; i++)
        vertical_frequency_distribution[i] = 0;

    for (j = 0; j < n; j++) {
        int vertical_line_length = 0;
        for (i = 0; i < n; i++) {
            if (R[i][j] == 1) {
                vertical_line_length += 1;
                if (i == (n - 1))
                    vertical_frequency_distribution[vertical_line_length] += 1;
            } else {
                if (vertical_line_length != 0) {
                    vertical_frequency_distribution[vertical_line_length] += 1;
                    vertical_line_length = 0;
                }
            }
        }
    }

    return vertical_frequency_distribution;
}

double compute_average_diagonal_length(int** R, int n) 
{
    double numerator = 0.0;
    double denominator = 0.0;
    int l_min = 2;

    // Calculate diagonal frequency distribution using the helper function
    int *diagonal_frequency_distribution = calculate_diagonal_frequency_distribution(R, n);
    if (diagonal_frequency_distribution == NULL) {
        // Handle error if allocation failed
        return 0.0;
    }

    // Calculate average diagonal length
    for (int l = l_min; l < n; l++) {
        numerator += l * diagonal_frequency_distribution[l];
        denominator += diagonal_frequency_distribution[l];
    }

    // Free the dynamically allocated memory
    free(diagonal_frequency_distribution);

    // Avoid division by zero
    return (denominator == 0) ? 0.0 : (double)numerator / denominator;
}

int compute_longest_diagonal_length(int **R, int n) 
{
    int *diagonal_frequency_distribution = calculate_diagonal_frequency_distribution(R, n);
    if (diagonal_frequency_distribution == NULL) {
        return 0;
    }

    int longest_diagonal_line_length = 0; // Initialize with 0
    for (int l = n - 1; l > 0; l--) {
        if (diagonal_frequency_distribution[l] != 0) {
            longest_diagonal_line_length = l;
            break;
        }
    }

    free(diagonal_frequency_distribution);
    
    return longest_diagonal_line_length;
}

double compute_average_vertical_length(int** R, int n) 
{
    double numerator   = 0.0;
    double denominator = 0.0;
    int v_min = 2;

    int *vertical_frequency_distribution = calculate_vertical_frequency_distribution(R, n);
    if (vertical_frequency_distribution == NULL) {
        return 0.0;
    }

    for (int v = v_min; v < n + 1; v++) {
        numerator += v * vertical_frequency_distribution[v];
        denominator += vertical_frequency_distribution[v];
    }

    // Free the dynamically allocated memory
    free(vertical_frequency_distribution);

    // Avoid division by zero
    return (denominator == 0) ? 0.0 : (double)numerator / denominator;
}

int compute_longest_vertical_length(int **R, int n) 
{
    int *vertical_frequency_distribution = calculate_vertical_frequency_distribution(R, n);
    if (vertical_frequency_distribution == NULL) {
        // Handle error if allocation failed
        return 0.0;
    }

    int longest_vertical_line_length;
    for (int v = n; v > 0; v--) {
        if (vertical_frequency_distribution[v] != 0) {
            longest_vertical_line_length = v;
            break;
        }
    }

    free(vertical_frequency_distribution);
    
    return longest_vertical_line_length;
}

double compute_determinism(int** R, int n) 
{
    int *diagonal_frequency_distribution = calculate_diagonal_frequency_distribution(R, n);
    int l_min = 2;

    if (diagonal_frequency_distribution == NULL) {
        // Handle error if allocation failed
        fprintf(stderr, "Memory allocation for diagonal_frequency_distribution failed.\n");
        return -1.0; // Return an error value
    }

    double numerator = 0.0;
    double denominator = 0.0;

    for (int l = l_min; l < n; l++) {
        numerator += l * diagonal_frequency_distribution[l];
    }
    
    for (int l = 1; l < n; l++) {
        denominator += l * diagonal_frequency_distribution[l];
    }

    // Free the dynamically allocated memory
    free(diagonal_frequency_distribution);

    // Avoid division by zero
    if (denominator == 0) {
        fprintf(stderr, "Denominator is zero, returning 0.0 to avoid division by zero.\n");
        return 0.0;
    }

    double determinism = numerator / denominator;

    // Ensure determinism is bounded by 1
    if (determinism > 1.0) determinism = 1.0;

    return determinism;
}

double compute_laminarity(int **R, int n) 
{
    int *vertical_frequency_distribution = calculate_vertical_frequency_distribution(R, n);
    int v_min = 2;

    if (vertical_frequency_distribution == NULL) {
        // Handle error if allocation failed
        fprintf(stderr, "Memory allocation for vertical_frequency_distribution failed.\n");
        return -1.0; // Return an error value
    }

    double numerator = 0.0;
    double denominator = 0.0;

    for (int v = v_min; v < n + 1; v++) {
        numerator += v * vertical_frequency_distribution[v];
    }

    for (int v = 1; v < n + 1; v++) {
        denominator += v * vertical_frequency_distribution[v];
    }

    free(vertical_frequency_distribution);

    // Avoid division by zero
    if (denominator == 0) {
        fprintf(stderr, "Denominator is zero, returning 0.0 to avoid division by zero.\n");
        return 0.0;
    }

    double laminarity = numerator / denominator;

    // Ensure laminarity is bounded by 1
    if (laminarity > 1.0) laminarity = 1.0;

    return laminarity;
}

double compute_divergence(int **R, int n) 
{
    return 1.0 / compute_longest_diagonal_length(R, n);
}

double compute_trapping_time(int **R, int n) 
{
    int *vertical_frequency_distribution = calculate_vertical_frequency_distribution(R, n);
    int v_min = 2;

    if (vertical_frequency_distribution == NULL) {
        // Handle error if allocation failed
        return 0.0;
    }

    double numerator = 0.0;
    double denominator = 0.0;

    for (int v = v_min; v <= n; v++) {
        numerator += v * vertical_frequency_distribution[v];
        denominator += vertical_frequency_distribution[v];
    }

    free(vertical_frequency_distribution);

    return (denominator == 0) ? 0.0 : (double)numerator / denominator;
}

double compute_entropy_diagonal(int **R, int n) 
{
    int *diagonal_frequency_distribution = calculate_diagonal_frequency_distribution(R, n);
    int l_min = 2;

    if (diagonal_frequency_distribution == NULL) {
        // Handle error if allocation failed
        return 0.0;
    }

    double sum_diagonal_frequency_distribution = 0.0;
    for(int l = l_min; l < n; l++)
    {
        sum_diagonal_frequency_distribution += diagonal_frequency_distribution[l];
    }
        
    double entropy_diagonal_lines = 0.0;

    if (sum_diagonal_frequency_distribution > 0.0)
    {
        for (int l = l_min; l < n; l++)
        {
            if (diagonal_frequency_distribution[l] != 0)
            {
                entropy_diagonal_lines += (diagonal_frequency_distribution[l]/sum_diagonal_frequency_distribution) * log(diagonal_frequency_distribution[l]/sum_diagonal_frequency_distribution);
            }
                
        }
        
        entropy_diagonal_lines *= -1.0;    
    }

    free(diagonal_frequency_distribution);

    return entropy_diagonal_lines;
}

double compute_entropy_vertical(int **R, int n)
{
    int *vertical_frequency_distribution = calculate_vertical_frequency_distribution(R, n);
    int v_min = 2;

    if (vertical_frequency_distribution == NULL) {
        // Handle error if allocation failed
        return 0.0;
    }

    double sum_vertical_frequency_distribution = 0.0;
    
    for (int v = v_min; v < n + 1; v++)
    {
        sum_vertical_frequency_distribution += vertical_frequency_distribution[v];
    }

    double entropy_vertical_lines = 0.0;

    if(sum_vertical_frequency_distribution > 0.0)
    {
        for (int v = v_min; v < n + 1; v++)
        {
            if (vertical_frequency_distribution[v] != 0)
            {
                entropy_vertical_lines += (vertical_frequency_distribution[v]/sum_vertical_frequency_distribution) * log(vertical_frequency_distribution[v]/sum_vertical_frequency_distribution);
            }
                
        }
        entropy_vertical_lines *= -1.0;    
    }

    free(vertical_frequency_distribution);

    return entropy_vertical_lines;
}


double wrap_longest_diagonal_length(int **matrix, int size) {
    return (double)compute_longest_diagonal_length(matrix, size);
}

double wrap_longest_vertical_length(int **matrix, int size) {
    return (double)compute_longest_vertical_length(matrix, size);
}