#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --------------------------
// 全局变量：统计比较次数
// --------------------------
long long bubble_cnt = 0;
long long merge_cnt = 0;
long long quick_cnt = 0;

// --------------------------
// 1. 冒泡排序
// --------------------------
void bubble_sort(int a[], int n) {
    bubble_cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bubble_cnt++;
            if (a[j] > a[j + 1]) {
                int t = a[j]; a[j] = a[j+1]; a[j+1] = t;
            }
        }
    }
}

// --------------------------
// 2. 归并排序
// --------------------------
void merge(int a[], int l, int mid, int r) {
    int *tmp = (int*)malloc((r-l+1)*sizeof(int));
    int i = l, j = mid+1, k = 0;
    while (i <= mid && j <= r) {
        merge_cnt++;
        if (a[i] < a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (i = 0; i < k; i++) a[l+i] = tmp[i];
    free(tmp);
}
void merge_sort(int a[], int l, int r) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    merge_sort(a, l, mid);
    merge_sort(a, mid+1, r);
    merge(a, l, mid, r);
}

// --------------------------
// 3. 快速排序
// --------------------------
int partition(int a[], int l, int r) {
    int p = a[l];
    int i = l+1, j = r;
    while (1) {
        while (i <= r) { quick_cnt++; if (a[i] > p) break; i++; }
        while (j > l) { quick_cnt++; if (a[j] < p) break; j--; }
        if (i >= j) break;
        int t = a[i]; a[i] = a[j]; a[j] = t;
    }
    a[l] = a[j]; a[j] = p;
    return j;
}
void quick_sort(int a[], int l, int r) {
    if (l >= r) return;
    int p = partition(a, l, r);
    quick_sort(a, l, p-1);
    quick_sort(a, p+1, r);
}

// --------------------------
// 随机数组生成
// --------------------------
void gen_array(int a[], int n) {
    for (int i = 0; i < n; i++) a[i] = rand() % 10000;
}

// =====================================================================
// 0-1 背包
// =================================================================----------

// 蛮力法（仅小n）
int brute_knapsack(int w[], int v[], int n, int C) {
    int maxv = 0;
    for (int mask = 0; mask < (1 << 20); mask++) {
        int tw = 0, tv = 0;
        for (int i = 0; i < n && i < 20; i++) {
            if (mask & (1 << i)) { tw += w[i]; tv += v[i]; }
        }
        if (tw <= C && tv > maxv) maxv = tv;
    }
    return maxv;
}

// 贪心
typedef struct { int w, v; double r; } Item;
int cmp(const void*a, const void*b) {
    Item x = *(Item*)a, y = *(Item*)b;
    return x.r > y.r ? -1 : 1;
}
int greedy_knapsack(int w[], int v[], int n, int C) {
    Item *its = (Item*)malloc(n*sizeof(Item));
    for (int i = 0; i < n; i++) { its[i].w=w[i]; its[i].v=v[i]; its[i].r=1.0*v[i]/w[i]; }
    qsort(its, n, sizeof(Item), cmp);
    int tw = 0, tv = 0;
    for (int i = 0; i < n; i++) {
        if (tw + its[i].w <= C) { tw += its[i].w; tv += its[i].v; }
    }
    free(its);
    return tv;
}

// 动态规划
int dp_knapsack(int w[], int v[], int n, int C) {
    int *dp = (int*)calloc(C+1, sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = C; j >= w[i]; j--) {
            if (dp[j - w[i]] + v[i] > dp[j]) dp[j] = dp[j - w[i]] + v[i];
        }
    }
    int ans = dp[C];
    free(dp);
    return ans;
}

// 回溯
int back_best;
void dfs(int i, int tw, int tv, int w[], int v[], int n, int C) {
    if (i == n) { if (tv > back_best) back_best = tv; return; }
    dfs(i+1, tw, tv, w, v, n, C);
    if (tw + w[i] <= C) dfs(i+1, tw + w[i], tv + v[i], w, v, n, C);
}
int back_knapsack(int w[], int v[], int n, int C) {
    back_best = 0;
    dfs(0, 0, 0, w, v, n, C);
    return back_best;
}

// =====================================================================
// 主函数
// =====================================================================
int main() {
    srand(time(0));
    printf("========== 算法实验 C语言 ==========\n");

    // ---------- 排序测试 ----------
    int ns[] = {10, 100, 1000};
    for (int k = 0; k < 3; k++) {
        int n = ns[k];
        int *a = (int*)malloc(n*sizeof(int));
        gen_array(a, n);

        int *b = (int*)malloc(n*sizeof(int));
        memcpy(b, a, n*sizeof(int));
        bubble_sort(b, n);

        memcpy(b, a, n*sizeof(int));
        merge_cnt = 0; merge_sort(b, 0, n-1);

        memcpy(b, a, n*sizeof(int));
        quick_cnt = 0; quick_sort(b, 0, n-1);

        printf("n=%4d | 冒泡:%lld | 归并:%lld | 快排:%lld\n",
               n, bubble_cnt, merge_cnt, quick_cnt);
        free(a); free(b);
    }

    // ---------- 背包测试 ----------
    printf("\n========== 0-1背包测试 ==========\n");
    int n = 1000, C = 10000;
    int *w = (int*)malloc(n*sizeof(int));
    int *v = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) { w[i] = rand()%100 + 1; v[i] = rand()%901 + 100; }

    clock_t st, ed;
    st = clock();
    int gv = greedy_knapsack(w, v, n, C);
    ed = clock();
    printf("贪心价值：%d 时间：%.2fms\n", gv, (double)(ed-st)/CLOCKS_PER_SEC*1000);

    st = clock();
    int dv = dp_knapsack(w, v, n, C);
    ed = clock();
    printf("DP价值：%d 时间：%.2fms\n", dv, (double)(ed-st)/CLOCKS_PER_SEC*1000);

    free(w); free(v);
    return 0;
}
