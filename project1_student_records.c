/*
 * ============================================================
 *  PROJECT 1: Dynamic Student Record Engine
 *  Student ID: jad6aR86d7N715dy
 *  Analysis Function: Grade Variance
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct Student {
    int    id;
    char   name[50];
    float  grades[5];   
    float  average;
};


struct Student *students = NULL;
int student_count = 0;
int capacity      = 0;


float compute_average(struct Student *s) {
    float sum = 0;
    for (int i = 0; i < 5; i++) sum += s->grades[i];
    return sum / 5.0f;
}


void add_student(struct Student **arr, int *count, int *cap) {
    if (*count >= *cap) {
        *cap = (*cap == 0) ? 4 : (*cap) * 2;
        *arr = realloc(*arr, (*cap) * sizeof(struct Student));
        if (!*arr) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
    }

    struct Student *s = &(*arr)[*count];

    printf("\n  Enter Student ID   : "); fflush(stdout); scanf("%d",  &s->id);
    printf("  Enter Student Name : "); fflush(stdout); scanf(" %49[^\n]", s->name);
    printf("  Enter 5 grades     : "); fflush(stdout);
    for (int i = 0; i < 5; i++) scanf("%f", &s->grades[i]);
    s->average = compute_average(s);

    (*count)++;
    printf("  Student record saved successfully.\n");
}

/* ══════════════════════════════════════════════════════════════
 *  DISPLAY ALL STUDENTS
 * ══════════════════════════════════════════════════════════════ */
void display_students(struct Student *arr, int count) {
    if (count == 0) { printf("\n  No student records found.\n"); return; }
    printf("\n  %-6s  %-28s  %7s %7s %7s %7s %7s  %8s\n",
           "ID", "Name", "Gr-1", "Gr-2", "Gr-3", "Gr-4", "Gr-5", "Average");
    printf("  %s\n", "-----------------------------------------------------------------------------");
    struct Student *p = arr;
    for (int i = 0; i < count; i++, p++) {
        printf("  %-6d  %-28s  %7.1f %7.1f %7.1f %7.1f %7.1f  %8.2f\n",
               p->id, p->name,
               p->grades[0], p->grades[1], p->grades[2],
               p->grades[3], p->grades[4], p->average);
    }
}


 //SORT STUDENTS by average grade, highest first (bubble sort)

void sort_students(struct Student *arr, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (arr[j].average < arr[j+1].average) {
                struct Student tmp = arr[j];
                arr[j]   = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
    printf("\n  Records sorted by average grade (highest to lowest).\n");
}


 //CLASS AVERAGE
 
void calculate_average(struct Student *arr, int count) {
    if (count == 0) { printf("\n  No records available.\n"); return; }
    float total = 0;
    for (int i = 0; i < count; i++) total += arr[i].average;
    printf("\n  Overall Class Average: %.2f\n", total / count);
}


 // FIND TOP STUDENT using pointer traversal

void find_top_student(struct Student *arr, int count) {
    if (count == 0) { printf("\n  No records available.\n"); return; }
    struct Student *best = arr;
    for (int i = 1; i < count; i++)
        if ((arr+i)->average > best->average) best = arr + i;
    printf("\n  Top Student : %s\n", best->name);
    printf("  Student ID  : %d\n", best->id);
    printf("  Average     : %.2f\n", best->average);
}

/* ══════════════════════════════════════════════════════════════
 *  CUSTOM ANALYSIS: Grade Variance
 *
 *  Variance measures how spread out a student's grades are.
 *  Formula: variance = sum of (grade - mean)^2  /  number of grades
 *
 *  Low variance  = grades are close together (consistent student)
 *  High variance = grades are all over the place (inconsistent)
 * ══════════════════════════════════════════════════════════════ */
void grade_variance(struct Student *arr, int count) {
    if (count == 0) { printf("\n  No records available.\n"); return; }
    printf("\n  %-28s  %8s  %10s  %s\n",
           "Name", "Average", "Variance", "Consistency");
    printf("  %s\n", "--------------------------------------------------------------");

    struct Student *p = arr;
    for (int i = 0; i < count; i++, p++) {
        float mean = p->average;
        float variance = 0;
        for (int j = 0; j < 5; j++) {
            float diff = p->grades[j] - mean;
            variance += diff * diff;
        }
        variance /= 5.0f;

        const char *label;
        if      (variance <= 20.0f)  label = "Very Consistent";
        else if (variance <= 60.0f)  label = "Consistent";
        else if (variance <= 120.0f) label = "Somewhat Inconsistent";
        else                         label = "Highly Inconsistent";

        printf("  %-28s  %8.2f  %10.2f  %s\n",
               p->name, p->average, variance, label);
    }
}


 // FUNCTION POINTER WRAPPERS
 //  These allow each menu option to be called through a pointer
 
typedef void (*op_fn)(struct Student *, int);

void menu_add(struct Student *a, int n)      { (void)a; (void)n; add_student(&students, &student_count, &capacity); }
void menu_display(struct Student *a, int n)  { display_students(a, n); }
void menu_sort(struct Student *a, int n)     { sort_students(a, n); }
void menu_avg(struct Student *a, int n)      { calculate_average(a, n); }
void menu_top(struct Student *a, int n)      { find_top_student(a, n); }
void menu_variance(struct Student *a, int n) { grade_variance(a, n); }

//Dataset
void load_records(void) {
    const char *names[] = {
        "Mercy Jepchumba",
        "Asanga Sonya Osengo",
        "Bruce Kimutai",
        "Lyndon Kipkemboi",
        "Felix Nyakamba",
        "Jesse Peter",
        "Odhiambo Raydexter Amayo",
        "Wawira Derick Munene",
        "June Mwende",
        "Denis Koech Kirwa",
        "Yvonne Mona Mruu",
        "George Momanyi",
        "Charlotte Ivy",
        "Kerina Maureen Kemunto",
        "Rebecca Ngoya"
    };
    float g[][5] = {
        {74, 81, 69, 78, 83},
        {88, 91, 85, 90, 87},
        {55, 62, 48, 57, 60},
        {90, 94, 88, 92, 95},
        {67, 71, 65, 70, 68},
        {80, 76, 83, 78, 82},
        {45, 52, 49, 55, 47},
        {93, 89, 96, 91, 94},
        {72, 68, 75, 70, 73},
        {61, 58, 64, 60, 63},
        {85, 88, 82, 87, 84},
        {77, 74, 79, 76, 80},
        {50, 46, 54, 48, 52},
        {96, 93, 98, 94, 97},
        {66, 70, 63, 68, 71}
    };

    capacity = 20;
    students = malloc(capacity * sizeof(struct Student));
    if (!students) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }

    for (int i = 0; i < 15; i++) {
        students[i].id = 2001 + i;
        strcpy(students[i].name, names[i]);
        for (int j = 0; j < 5; j++) students[i].grades[j] = g[i][j];
        students[i].average = compute_average(&students[i]);
    }
    student_count = 15;
}


 // MAIN
 
int main(void) {
    printf("\n");
    printf("  ╔════════════════════════════════════════════════════════╗\n");
    printf("  ║         DYNAMIC STUDENT RECORD ENGINE                 ║\n");
    printf("  ║         Student ID : jad6aR86d7N715dy                 ║\n");
    printf("  ║         Analysis   : Grade Variance                   ║\n");
    printf("  ╚════════════════════════════════════════════════════════╝\n");

    load_records();

    
    op_fn ops[] = {
        NULL,          
        menu_add,      
        menu_display,  
        menu_sort,     
        menu_avg,      
        menu_top,      
        menu_variance, 
    };

    int choice;
    do {
        printf("\n  ┌──────────────────────────────────────┐\n");
        printf("  │              MAIN MENU               │\n");
        printf("  ├──────────────────────────────────────┤\n");
        printf("  │  1. Add Student                      │\n");
        printf("  │  2. Display Students                 │\n");
        printf("  │  3. Sort by Average Grade            │\n");
        printf("  │  4. Calculate Class Average          │\n");
        printf("  │  5. Find Top Student                 │\n");
        printf("  │  6. Grade Variance Analysis          │\n");
        printf("  │  0. Exit                             │\n");
        printf("  └──────────────────────────────────────┘\n");
        printf("  Choice: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= 6) {
            ops[choice](students, student_count);
        } else if (choice != 0) {
            printf("  Invalid option. Please enter 0-6.\n");
        }

    } while (choice != 0);

    printf("\n  Releasing memory...\n");
    free(students);
    return 0;
}