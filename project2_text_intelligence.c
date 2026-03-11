#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct WordStat {
    char name[40];
    int  count;
};


char            *text_buffer  = NULL;
char           **word_array   = NULL;
int              word_count   = 0;
struct WordStat *word_stats   = NULL;
int              unique_count = 0;

/* ══════════════════════════════════════════════════════════════
 *  split_into_words: scan each character in the buffer using
 *  a pointer and extract every alphabetic word into word_array
 * ══════════════════════════════════════════════════════════════ */
void split_into_words(char *buf) {
    /* first pass  */
    int   total = 0;
    char *ptr   = buf;
    int   in    = 0;
    while (*ptr) {
        if (isalpha((unsigned char)*ptr)) { if (!in) { total++; in = 1; } }
        else { in = 0; }
        ptr++;
    }

    word_array = malloc(total * sizeof(char *));
    if (!word_array) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }

    /* second pass — copy each word in lowercase into word_array */
    ptr = buf; in = 0;
    int idx    = 0;
    char *start = NULL;
    while (*ptr) {
        if (isalpha((unsigned char)*ptr)) {
            if (!in) { start = ptr; in = 1; }
        } else {
            if (in) {
                int len = (int)(ptr - start);
                word_array[idx] = malloc((len + 1) * sizeof(char));
                strncpy(word_array[idx], start, len);
                word_array[idx][len] = '\0';
                for (int k = 0; k < len; k++)
                    word_array[idx][k] = (char)tolower((unsigned char)word_array[idx][k]);
                idx++;
                in = 0;
            }
        }
        ptr++;
    }
    /* handle last word if the text ends without punctuation */
    if (in) {
        int len = (int)(ptr - start);
        word_array[idx] = malloc((len + 1) * sizeof(char));
        strncpy(word_array[idx], start, len);
        word_array[idx][len] = '\0';
        for (int k = 0; k < len; k++)
            word_array[idx][k] = (char)tolower((unsigned char)word_array[idx][k]);
        idx++;
    }
    word_count = idx;
}

/* ══════════════════════════════════════════════════════════════
 *  count_frequencies: go through every word and use strcmp
 *  to check if it already exists in word_stats. if yes,
 *  increment its count. if no, add it as a new entry.
 * ══════════════════════════════════════════════════════════════ */
void count_frequencies(void) {
    int cap  = 64;
    word_stats = malloc(cap * sizeof(struct WordStat));
    if (!word_stats) { fprintf(stderr, "Memory allocation failed\n"); exit(1); }
    unique_count = 0;

    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(word_stats[j].name, word_array[i]) == 0) {
                word_stats[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (unique_count >= cap) {
                cap *= 2;
                word_stats = realloc(word_stats, cap * sizeof(struct WordStat));
            }
            strcpy(word_stats[unique_count].name, word_array[i]);
            word_stats[unique_count].count = 1;
            unique_count++;
        }
    }
}

/* ══════════════════════════════════════════════════════════════
 *  count_words: display total and unique word counts
 * ══════════════════════════════════════════════════════════════ */
void count_words(void) {
    printf("\n  Total words  : %d\n", word_count);
    printf("  Unique words : %d\n", unique_count);
}

/* ══════════════════════════════════════════════════════════════
 *  longest_word: use strlen to measure each word and
 *  track whichever one is longest
 * ══════════════════════════════════════════════════════════════ */
void longest_word(void) {
    int   best_len = 0;
    char *best     = NULL;
    for (int i = 0; i < word_count; i++) {
        int l = (int)strlen(word_array[i]);
        if (l > best_len) { best_len = l; best = word_array[i]; }
    }
    printf("\n  Longest word : \"%s\"  (%d characters)\n",
           best ? best : "none", best_len);
}

/* ══════════════════════════════════════════════════════════════
 *  most_frequent: find the word that appears the most,
 *  then print the top 10 most common words overall
 * ══════════════════════════════════════════════════════════════ */
void most_frequent(void) {
    int   best_cnt = 0;
    char *best     = NULL;
    for (int i = 0; i < unique_count; i++) {
        if (word_stats[i].count > best_cnt) {
            best_cnt = word_stats[i].count;
            best     = word_stats[i].name;
        }
    }
    printf("\n  Most frequent word : \"%s\"  (%d times)\n",
           best ? best : "none", best_cnt);

    int n = (unique_count < 10) ? unique_count : 10;
    int shown[10];
    for (int s = 0; s < n; s++) shown[s] = -1;

    printf("\n  Top %d words:\n", n);
    for (int s = 0; s < n; s++) {
        int maxc = -1, maxi = -1;
        for (int j = 0; j < unique_count; j++) {
            int already = 0;
            for (int k = 0; k < s; k++) if (shown[k] == j) { already = 1; break; }
            if (!already && word_stats[j].count > maxc) { maxc = word_stats[j].count; maxi = j; }
        }
        if (maxi >= 0) {
            shown[s] = maxi;
            printf("    %-20s %d\n", word_stats[maxi].name, word_stats[maxi].count);
        }
    }
}

/* ══════════════════════════════════════════════════════════════
 *  CUSTOM ANALYSIS: Vowel Density
 *
 *  Scans every character in the text buffer using a pointer.
 *  Counts how many are vowels (a, e, i, o, u) vs total letters.
 *  Expresses this as a percentage and breaks it down per vowel.
 * ══════════════════════════════════════════════════════════════ */
void vowel_density(void) {
    int total_letters = 0;
    int total_vowels  = 0;
    int counts[5]     = {0, 0, 0, 0, 0};
    const char vowels[] = "aeiou";

    char *ptr = text_buffer;
    while (*ptr) {
        if (isalpha((unsigned char)*ptr)) {
            total_letters++;
            char c = (char)tolower((unsigned char)*ptr);
            for (int v = 0; v < 5; v++) {
                if (c == vowels[v]) {
                    counts[v]++;
                    total_vowels++;
                    break;
                }
            }
        }
        ptr++;
    }

    float density = (total_letters > 0)
                    ? (float)total_vowels / total_letters * 100.0f : 0.0f;

    printf("\n  Vowel Density Analysis\n");
    printf("  --------------------------------\n");
    printf("  Total letters : %d\n", total_letters);
    printf("  Total vowels  : %d\n", total_vowels);
    printf("  Vowel density : %.1f%%\n", density);
    printf("\n  Breakdown by vowel:\n");
    for (int v = 0; v < 5; v++) {
        float pct = (total_letters > 0)
                    ? (float)counts[v] / total_letters * 100.0f : 0.0f;
        printf("    '%c'  :  %3d occurrences  (%.1f%% of all letters)\n",
               vowels[v], counts[v], pct);
    }

    const char *rating;
    if      (density >= 45.0f) rating = "Very vowel-rich text";
    else if (density >= 38.0f) rating = "Typical vowel balance";
    else                       rating = "Consonant-heavy text";
    printf("\n  Rating : %s\n", rating);
}

/* ══════════════════════════════════════════════════════════════
 *  print_memory_info: show where key structures live in memory
 * ══════════════════════════════════════════════════════════════ */
void print_memory_info(void) {
    printf("\n  Memory addresses of key data structures:\n");
    printf("    text_buffer stored at : %p\n", (void *)text_buffer);
    printf("    word_array  stored at : %p\n", (void *)word_array);
    printf("    word_stats  stored at : %p\n", (void *)word_stats);
    if (word_count > 0)
        printf("    word_array[0]         : %p  -> \"%s\"\n",
               (void *)word_array[0], word_array[0]);
}

/* ══════════════════════════════════════════════════════════════
 *  cleanup_memory: release all dynamically allocated memory
 * ══════════════════════════════════════════════════════════════ */
void cleanup_memory(void) {
    if (word_array) {
        for (int i = 0; i < word_count; i++) free(word_array[i]);
        free(word_array);
        word_array = NULL;
    }
    free(word_stats);  word_stats  = NULL;
    free(text_buffer); text_buffer = NULL;
    word_count = unique_count = 0;
}

/* function pointer type for analysis functions */
typedef void (*analysis_fn)(void);

/* ══════════════════════════════════════════════════════════════
 *  MAIN
 * ══════════════════════════════════════════════════════════════ */
int main(void) {
    setbuf(stdout, NULL);

    /* paragraph from To Kill a Mockingbird by Harper Lee */
    const char *default_text =
        "I'd rather you shot at tin cans in the backyard, but I know you'll "
        "go after birds. Shoot all the blue jays you want, if you can hit em, "
        "but remember it's a sin to kill a mockingbird. That was the only time "
        "I ever heard Atticus say it was a sin to do something, and I asked "
        "Miss Maudie about it. Your father's right, she said. Mockingbirds "
        "don't do one thing except make music for us to enjoy. They don't eat "
        "up people's gardens, don't nest in corn cribs, they don't do one thing "
        "but sing their hearts out for us. That's why it's a sin to kill a "
        "mockingbird.";

    printf("\n");
    printf("  +----------------------------------------------------------+\n");
    printf("  |        ADAPTIVE TEXT INTELLIGENCE TOOL                  |\n");
    printf("  |        Student ID : jad6aR86d7N715dy                    |\n");
    printf("  |        Custom     : Vowel Density                       |\n");
    printf("  |        Source     : To Kill a Mockingbird - Harper Lee  |\n");
    printf("  +----------------------------------------------------------+\n");

    analysis_fn fns[] = {
        NULL,             /* 0 placeholder */
        NULL,             /* 1 load default — handled below */
        NULL,             /* 2 custom input — handled below */
        count_words,      /* 3 */
        longest_word,     /* 4 */
        most_frequent,    /* 5 */
        vowel_density,    /* 6 */
        print_memory_info /* 7 */
    };

    int choice;
    do {
        printf("\n  +--------------------------------------+\n");
        printf("  |             MAIN MENU               |\n");
        printf("  +--------------------------------------+\n");
        printf("  |  1. Load default text               |\n");
        printf("  |  2. Enter your own text             |\n");
        printf("  |  3. Count Words                     |\n");
        printf("  |  4. Longest Word                    |\n");
        printf("  |  5. Most Frequent Word              |\n");
        printf("  |  6. Vowel Density (custom)          |\n");
        printf("  |  7. Show Memory Addresses           |\n");
        printf("  |  0. Exit                            |\n");
        printf("  +--------------------------------------+\n");
        printf("  Choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            cleanup_memory();
            int len = (int)strlen(default_text);
            text_buffer = malloc((len + 1) * sizeof(char));
            strcpy(text_buffer, default_text);
            split_into_words(text_buffer);
            count_frequencies();
            printf("\n  Text loaded: %d words, %d unique.\n",
                   word_count, unique_count);

        } else if (choice == 2) {
            cleanup_memory();
            printf("\n  Paste your text then press Enter:\n  > ");
            text_buffer = malloc(3000 * sizeof(char));
            fgets(text_buffer, 2999, stdin);
            text_buffer[strcspn(text_buffer, "\n")] = '\0';
            split_into_words(text_buffer);
            count_frequencies();
            printf("  Text loaded: %d words, %d unique.\n",
                   word_count, unique_count);

        } else if (choice >= 3 && choice <= 7) {
            if (!text_buffer) {
                printf("\n  Please load text first (option 1 or 2).\n");
            } else {
                fns[choice]();
            }
        } else if (choice != 0) {
            printf("  Invalid option. Please enter 0-7.\n");
        }

    } while (choice != 0);

    cleanup_memory();
    printf("\n  Memory released. Goodbye.\n");
    return 0;
}