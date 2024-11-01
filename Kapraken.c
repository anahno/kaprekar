#include <stdio.h>
#include <stdlib.h>

// تعداد ارقام را به صورت یک متغیر ثابت تعریف می‌کنیم
#define DIGITS 6

// تعریف ساختار برای ذخیره اعداد و شمارش آن‌ها
typedef struct {
    int num;
    int count;
} RepeatedNumber;

// تعریف ساختار برای ذخیره اعداد مشاهده شده
typedef struct Node {
    int num;
    struct Node *next;
} Node;

// تابع اضافه کردن عدد به لیست مشاهده شده‌ها
void add_seen(Node **head, int num) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "خطا در تخصیص حافظه!\n");
        exit(1);
    }
    new_node->num = num;
    new_node->next = *head;
    *head = new_node;
}

// تابع بررسی تکراری بودن عدد در لیست مشاهده شده‌ها
int is_seen(Node *head, int num) {
    Node *current = head;
    while (current != NULL) {
        if (current->num == num) return 1;
        current = current->next;
    }
    return 0;
}

int compare_desc(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

int compare_asc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// تابع بررسی ورودی معتبر
int check_valid_input(int num) {
    int digits[10] = {0};
    int count_repeats = 0;

    // شمارش تعداد تکرار ارقام
    while (num > 0) {
        digits[num % 10]++;
        num /= 10;
    }
    
    for (int i = 0; i < 10; i++) {
        if (digits[i] > 1) {
            count_repeats += digits[i] - 1;
        }
    }
    return count_repeats <= 2; // حداکثر دو رقم مشابه مجاز است
}

// الگوریتم اصلی
void perform_algorithm(int num, RepeatedNumber *repeated_numbers, int *repeated_count) {
    int digits[DIGITS];
    Node *seen = NULL;

    while (1) {
        if (is_seen(seen, num)) {
            for (int i = 0; i < *repeated_count; i++) {
                if (repeated_numbers[i].num == num) {
                    repeated_numbers[i].count++;
                    return; // اگر عدد تکراری بود، فقط شمارش را افزایش بده
                }
            }
            // اگر عدد جدید بود
            repeated_numbers[*repeated_count].num = num;
            repeated_numbers[*repeated_count].count = 1;
            (*repeated_count)++;
            printf("عدد %d به چرخه تکراری رسید.\n", num); // پیام دیباگ

            // آزاد کردن حافظه لیست پیوندی
            while (seen != NULL) {
                Node *temp = seen;
                seen = seen->next;
                free(temp);
            }
            return;
        }

        add_seen(&seen, num);

        int temp = num;
        for (int i = 0; i < DIGITS; i++) {
            digits[i] = temp % 10;
            temp /= 10;
        }

        qsort(digits, DIGITS, sizeof(int), compare_desc);
        int large_num = 0;
        for (int i = 0; i < DIGITS; i++) {
            large_num = large_num * 10 + digits[i];
        }

        qsort(digits, DIGITS, sizeof(int), compare_asc);
        int small_num = 0;
        for (int i = 0; i < DIGITS; i++) {
            small_num = small_num * 10 + digits[i];
        }

        num = large_num - small_num;
    }
}

int main() {
    // ساخت نام فایل بر اساس تعداد ارقام
    char filename[20];
    sprintf(filename, "output%d.txt", DIGITS);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("خطا در باز کردن فایل برای نوشتن!\n");
        return 1;
    }

    RepeatedNumber repeated_numbers[10000];
    int repeated_count = 0;

    for (int num = 100000; num < 1000000; num++) {
        if (check_valid_input(num)) {
            perform_algorithm(num, repeated_numbers, &repeated_count);
        }
    }

    fprintf(file, "%-10s %-10s %-10s\n", "Number", "Value", "Frequency");
    fprintf(file, "---------- ---------- ----------\n");
    for (int i = 0; i < repeated_count; i++) {
        fprintf(file, "%d\t   %d      %d\n", i + 1, repeated_numbers[i].num, repeated_numbers[i].count);
    }

    fclose(file);
    printf("خروجی در فایل %s ذخیره شد.\n", filename);

    return 0;
}
