/* --------------------------------------------------------------------------
DESCRIPTION: Recurrence Quantification Analysis (RQA) from a given .dat file.
OBSERVATIONS: The idea is that (for now) the .dat is a two collumn file, 
like an orbit of a dynamical system.
* -------------------------------------------------------------------------- */
#include "lib/common.h"
#include "general_RQA_from_file.h"

#include "lib/recurrence_functions.h"
#include "lib/auxiliary_functions.h"
#include "lib/gnuplot_functions.h"

/* --------------------------------------------------------------------------
 * GLOBAL PARAMETERS - RECURRENCE ANALYSIS
 * --------------------------------------------------------------------------
 * These parameters control the recurrence analysis process. 
 * - `eps`: Recurrence threshold.
 * - `emb_dim`: Embedding dimension.
 * - `tau`: Time delay.
 * - `percentage`: Percentage for threshold adjustment (if enabled).
 * - `define_threshold_via_percentage`: Whether to adjust threshold via percentage.
 * -------------------------------------------------------------------------- */
double eps = 0.2;
int emb_dim = 2;
int tau = 1;
double percentage = 20;
bool define_threshold_via_percentage = false;

// Default path for storing results
const char *results_path = "RQA_results";

/* --------------------------------------------------------------------------
 * FUNCTION: count_lines_in_file
 * PURPOSE:  Determines the number of lines in a given .dat file
 * -------------------------------------------------------------------------- */
int count_lines_in_file(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "❌ ERROR: Unable to open file '%s' to count lines.\n", filename);
        return -1;
    }

    int count = 0;
    char buffer[1024]; // Temporary buffer for reading lines
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
    }

    fclose(file);
    return count;
}


/* --------------------------------------------------------------------------
 * FUNCTION: give_recurrence_plot
 * PURPOSE:  Generates and saves the recurrence plot data.
 * -------------------------------------------------------------------------- */
int give_recurrence_plot(int **R, int N) 
{
    FILE *rm = fopen("rm.dat", "w");
    FILE *rp = fopen("rp.dat", "w");

    if (rm == NULL || rp == NULL) {
        fprintf(stderr, "❌ ERROR: Unable to open files for writing.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(rm, "%d ", R[i][j]);
            if (R[i][j] == 1 && i >= j) {
                fprintf(rp, "%d %d %d\n", i, j, i);
            }
        }
        fprintf(rm, "\n");
    }

    fclose(rm);
    fclose(rp);
    return 0;
}

/* --------------------------------------------------------------------------
 * FUNCTION: calculate_all_recurrence_quantifiers
 * PURPOSE:  Computes all recurrence quantifiers and saves them to files.
 * -------------------------------------------------------------------------- */
void calculate_all_recurrence_quantifiers(int **R, int N) 
{
    size_t folder_name_size = strlen(results_path) + 256;
    char *folder_name = (char *)malloc(folder_name_size);
    snprintf(folder_name, folder_name_size, "%s/", results_path);
    create_folders_in_path_if_not_exists(folder_name);

    printf("Calculating Recurrence Quantifiers...\n");

    char filename[256];
    FILE *out;

    struct {
        const char *name;
        double (*func)(int **, int);
    } quantifiers[] = {
        {"RR", compute_recurrence_rate},
        {"DET", compute_determinism},
        {"LAM", compute_laminarity},
        {"TT", compute_trapping_time},
        {"ENTR_L", compute_entropy_diagonal},
        {"ENTR_V", compute_entropy_vertical},
        {"L_MED", compute_average_diagonal_length},
        {"V_MED", compute_average_vertical_length},
        {"DIV", compute_divergence},
        {"L_MAX", wrap_longest_diagonal_length},
        {"V_MAX", wrap_longest_vertical_length}, 
    };

    for (int i = 0; i < 11; i++) 
    {
        double value = quantifiers[i].func(R, N);
        printf("%s = %f\n", quantifiers[i].name, value);
    }

    free(folder_name);
}

/* --------------------------------------------------------------------------
 * FUNCTION: compute_recurrence_quantifiers_over_time
 * PURPOSE:  Computes recurrence quantifiers over time and saves them to files.
 * -------------------------------------------------------------------------- */
void compute_recurrence_quantifiers_over_time(int **R, int N, int window_size, int step_size) 
{
    printf("Computing Recurrence Quantifiers Over Time...\n");

    if (window_size > N || step_size <= 0) {
        fprintf(stderr, "❌ ERROR: Invalid window size or step size\n");
        return;
    }

    size_t folder_name_size = strlen(results_path) + 256;
    char *folder_name = (char *)malloc(folder_name_size);
    snprintf(folder_name, folder_name_size, "%s/", results_path);
    create_folders_in_path_if_not_exists(folder_name);

    char filename[256];
    FILE *files[11];
    const char *quantifier_names[] = {
        "RR_over_time", "DET_over_time", "LAM_over_time", "DIV_over_time",
        "TT_over_time", "L_MED_over_time", "L_MAX_over_time",
        "V_MED_over_time", "V_MAX_over_time", "ENTR_L_over_time", "ENTR_V_over_time"
    };

    for (int i = 0; i < 11; i++) {
        snprintf(filename, sizeof(filename), "%s%s.dat", folder_name, quantifier_names[i]);
        files[i] = fopen(filename, "w");
        if (!files[i]) {
            fprintf(stderr, "❌ ERROR: Unable to open file: %s\n", filename);
            free(folder_name);
            return;
        }
    }

    for (int start = 0; start <= N - window_size; start += step_size) {
        int **window_R;
        alloc_2d_int(&window_R, window_size, window_size);

        for (int i = 0; i < window_size; i++)
            for (int j = 0; j < window_size; j++)
                window_R[i][j] = R[start + i][start + j];

        double values[] = {
            compute_recurrence_rate(window_R, window_size),
            compute_determinism(window_R, window_size),
            compute_laminarity(window_R, window_size),
            compute_divergence(window_R, window_size),
            compute_trapping_time(window_R, window_size),
            compute_average_diagonal_length(window_R, window_size),
            (double)compute_longest_diagonal_length(window_R, window_size),
            compute_average_vertical_length(window_R, window_size),
            (double)compute_longest_vertical_length(window_R, window_size),
            compute_entropy_diagonal(window_R, window_size),
            compute_entropy_vertical(window_R, window_size),
        };

        for (int i = 0; i < 11; i++)
            fprintf(files[i], "%d %f\n", start, values[i]);

        dealloc_2d_int(&window_R, window_size);
    }

    for (int i = 0; i < 11; i++)
        fclose(files[i]);

    free(folder_name);
}

/* --------------------------------------------------------------------------
 * FUNCTION: recurrence_quantification_analysis
 * PURPOSE:  Reads the input data, computes the recurrence matrix, 
 *           and performs recurrence quantification analysis.
 * -------------------------------------------------------------------------- */
void recurrence_quantification_analysis(const char *input_file, double eps) 
{
    struct stat buffer;
    if (stat(input_file, &buffer) != 0) {
        fprintf(stderr, "❌ ERROR: Input file '%s' not found! Please provide a valid .dat file.\n", input_file);
        return;
    }

    // Determine the number of points (N) dynamically
    int N = count_lines_in_file(input_file);
    if (N <= 0) {
        fprintf(stderr, "❌ ERROR: No valid data found in '%s'.\n", input_file);
        return;
    }
    printf("✅ Detected %d lines in '%s'. Setting N = %d\n", N, input_file, N);

    FILE *file = fopen(input_file, "r");
    if (!file) {
        fprintf(stderr, "❌ ERROR: Unable to open input file '%s'.\n", input_file);
        return;
    }

    // Allocate memory for orbit data
    double **orbit;
    alloc_2d_double(&orbit, N, 2);

    for (int i = 0; i < N; i++)
        fscanf(file, "%lf %lf", &orbit[i][0], &orbit[i][1]);
    fclose(file);

    // Compute distance matrix and recurrence matrix
    double **DM;
    int **RM;
    alloc_2d_double(&DM, N, N);
    calculate_distance_matrix(orbit, N, 2, DM, "euclidean");
    dealloc_2d_double(&orbit, N);

    alloc_2d_int(&RM, N, N);
    compute_recurrence_matrix(DM, N, RM, eps);

    //Plot the Recurrence Plot
    plot_gnuplot_RPs_title("rp.png", N, tau, emb_dim, eps);

    // Generate and save recurrence plots
    give_recurrence_plot(RM, N);
    calculate_all_recurrence_quantifiers(RM, N);
    compute_recurrence_quantifiers_over_time(RM, N, 100, 10);

    // Clean up memory
    dealloc_2d_double(&DM, N);
    dealloc_2d_int(&RM, N);
}

/* --------------------------------------------------------------------------
 * MAIN FUNCTION
 * PURPOSE:  Entry point of the program, calling the main recurrence 
 *           quantification analysis function.
 * -------------------------------------------------------------------------- */
int main() {
    printf("📌 Starting Recurrence Quantification Analysis...\n");
    recurrence_quantification_analysis("data.dat", eps);
    printf("✅ Analysis Completed Successfully!\n");
    return 0;
}
