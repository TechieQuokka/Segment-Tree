# Segment Tree (구간 트리) - C 언어 구현

**Lazy Propagation 알고리즘**을 포함한 고성능 Segment Tree 라이브러리로, 배열의 범위 쿼리와 범위 업데이트를 효율적으로 처리할 수 있습니다.

## 🌟 주요 특징

- **효율적인 범위 쿼리**: O(log n) 시간 복잡도
- **다양한 연산 지원**: 합, 최솟값, 최댓값, 사용자 정의 연산
- **⚡ Lazy Propagation (지연 전파)**: 범위 업데이트를 O(log n)으로 최적화
- **두 가지 모드 지원**: 기본 모드와 지연 전파 모드 선택 가능
- **메모리 최적화**: 효율적인 메모리 사용
- **견고한 오류 처리**: 포괄적인 오류 검증
- **성능 통계**: 쿼리/업데이트 성능 모니터링
- **ASCII 전용**: 순수 ASCII 코드로 구현

## 🎯 알고리즘 특징

### 기본 Segment Tree vs Lazy Propagation

| 연산 유형 | 기본 모드 | Lazy Propagation 모드 | 성능 향상 |
|-----------|-----------|----------------------|-----------|
| **점 업데이트** | O(log n) | O(log n) | 동일 |
| **범위 업데이트** | O(n log n) | **O(log n)** | 🚀 **대폭 향상** |
| **범위 쿼리** | O(log n) | O(log n) | 동일 |
| **메모리 사용** | O(4n) | O(8n) | 2배 (여전히 효율적) |

### 🔥 언제 Lazy Propagation을 사용해야 할까?

- ✅ **범위 업데이트가 빈번한 경우** (예: 구간에 값 추가/설정)
- ✅ **실시간 시스템** (센서 데이터 일괄 업데이트)
- ✅ **게임/시뮬레이션** (영역별 속성 변경)
- ✅ **금융 시스템** (구간별 수수료/이자 적용)

## 📁 프로젝트 구조

```
segment-tree/
├── include/
│   └── segment_tree.h          # 공용 헤더 파일
├── src/
│   ├── segment_tree.c          # 핵심 구현
│   ├── operations.c            # 연산 함수들
│   └── utils.c                 # 유틸리티 함수들
├── tests/
│   ├── test_basic.c           # 기본 기능 테스트
│   ├── test_lazy.c            # 지연 전파 테스트
│   └── test_performance.c      # 성능 테스트
├── examples/
│   ├── example_sum.c          # 구간 합 예제
│   ├── example_min_max.c      # 최솟값/최댓값 예제
│   └── example_lazy.c         # 지연 전파 예제
├── docs/
│   ├── segment_tree_architecture.md      # 아키텍처 설계
│   └── segment_tree_implementation_paper.md  # 구현 논문
├── Makefile
└── README.md
```

## 🚀 빠른 시작

### 1. 빌드

```bash
# 전체 빌드 (라이브러리, 테스트, 예제)
make all

# 라이브러리만 빌드
make lib

# 디버그 모드로 빌드
make debug

# 릴리즈 모드로 빌드
make release
```

### 2. 테스트 실행

```bash
# 모든 테스트 실행
make run-tests

# 개별 테스트 실행
./bin/test_basic
./bin/test_lazy
./bin/test_performance
```

### 3. 예제 실행

```bash
# 모든 예제 실행
make run-examples

# 개별 예제 실행
./bin/example_sum
./bin/example_min_max
./bin/example_lazy
```

## 💻 사용법

### 기본 사용 예제

```c
#include "segment_tree.h"

int main() {
    // 배열 초기화
    int arr[] = {1, 3, 5, 7, 9};
    int n = 5;

    // Segment Tree 생성 (구간 합)
    segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);

    // 범위 쿼리
    int result;
    segtree_query(st, 1, 3, &result);  // 인덱스 1~3의 합
    printf("Sum of range [1, 3]: %d\n", result);

    // 점 업데이트
    segtree_update_point(st, 2, 10);   // 인덱스 2를 10으로 변경

    // 메모리 해제
    segtree_destroy(st);

    return 0;
}
```

### 지연 전파 사용 예제

```c
#include "segment_tree.h"

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = 5;

    // 지연 전파가 활성화된 Segment Tree 생성
    segment_tree_t *st = segtree_create_lazy(arr, n, SEGTREE_SUM);

    // 범위 업데이트 (인덱스 1~3에 모두 5 추가)
    segtree_update_range(st, 1, 3, 5);

    // 범위 쿼리
    int result;
    segtree_query(st, 0, 4, &result);
    printf("Total sum after range update: %d\n", result);

    segtree_destroy(st);
    return 0;
}
```

### 사용자 정의 연산

```c
#include "segment_tree.h"

// 곱셈 연산 정의
int multiply(int a, int b) {
    return a * b;
}

int main() {
    int arr[] = {2, 3, 4, 5};
    int n = 4;

    // 사용자 정의 연산으로 Segment Tree 생성
    segment_tree_t *st = segtree_create_custom(arr, n, multiply, 1);

    int result;
    segtree_query(st, 0, 3, &result);  // 2 * 3 * 4 * 5 = 120
    printf("Product of all elements: %d\n", result);

    segtree_destroy(st);
    return 0;
}
```

## 📚 API 참조

### 생성 및 소멸

```c
// 기본 생성자
segment_tree_t* segtree_create(int *arr, int n, segtree_operation_t op);

// 사용자 정의 연산
segment_tree_t* segtree_create_custom(int *arr, int n,
                                     int (*combine)(int, int), int identity);

// 지연 전파 지원
segment_tree_t* segtree_create_lazy(int *arr, int n, segtree_operation_t op);

// 소멸자
void segtree_destroy(segment_tree_t *st);
```

### 쿼리 및 업데이트

```c
// 범위 쿼리
segtree_error_t segtree_query(segment_tree_t *st, int left, int right, int *result);

// 점 업데이트
segtree_error_t segtree_update_point(segment_tree_t *st, int index, int value);

// 범위 업데이트 (지연 전파 필요)
segtree_error_t segtree_update_range(segment_tree_t *st, int left, int right, int value);
```

### 유틸리티

```c
// 트리 출력 (디버깅용)
void segtree_print(segment_tree_t *st);

// 트리 유효성 검증
bool segtree_validate(segment_tree_t *st);

// 성능 통계
segtree_stats_t segtree_get_stats(segment_tree_t *st);
void segtree_reset_stats(segment_tree_t *st);
```

## 🔧 연산 타입

| 타입 | 설명 | 항등원 |
|------|------|--------|
| `SEGTREE_SUM` | 구간 합 | 0 |
| `SEGTREE_MIN` | 구간 최솟값 | INT_MAX |
| `SEGTREE_MAX` | 구간 최댓값 | INT_MIN |
| `SEGTREE_CUSTOM` | 사용자 정의 | 사용자 지정 |

## ⚡ 성능 특성

### 시간 복잡도
- **생성**: O(n)
- **쿼리**: O(log n)
- **점 업데이트**: O(log n)
- **범위 업데이트**: O(log n) (지연 전파 사용시)

### 공간 복잡도
- **기본**: O(4n) ≈ O(n)
- **지연 전파**: O(8n) ≈ O(n)

### 벤치마크 결과

```
배열 크기: 100,000 요소
- 생성 시간: ~0.002초
- 쿼리 시간: ~0.000001초 (1마이크로초)
- 업데이트 시간: ~0.000002초 (2마이크로초)
```

## 🛠️ 고급 사용법

### 성능 모니터링

```c
segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);

// 여러 연산 수행...

// 통계 확인
segtree_stats_t stats = segtree_get_stats(st);
printf("총 쿼리: %d, 평균 쿼리 시간: %.6f초\n",
       stats.query_count,
       stats.total_query_time / stats.query_count);
```

### 오류 처리

```c
int result;
segtree_error_t error = segtree_query(st, left, right, &result);

switch (error) {
    case SEGTREE_OK:
        printf("결과: %d\n", result);
        break;
    case SEGTREE_ERROR_INVALID_RANGE:
        printf("오류: 잘못된 범위입니다.\n");
        break;
    case SEGTREE_ERROR_NULL_POINTER:
        printf("오류: NULL 포인터입니다.\n");
        break;
    default:
        printf("알 수 없는 오류가 발생했습니다.\n");
        break;
}
```

## 🧪 테스트

### 단위 테스트

```bash
# 기본 기능 테스트
make && ./bin/test_basic

# 지연 전파 테스트
make && ./bin/test_lazy

# 성능 테스트
make && ./bin/test_performance
```

### 메모리 누수 검사

```bash
# Valgrind로 메모리 검사
make valgrind-tests
```

## 📦 설치

### 시스템 전체 설치

```bash
make install
```

### 제거

```bash
make uninstall
```

### 수동 설치

```bash
# 라이브러리 빌드
make lib

# 헤더 파일 복사
cp include/segment_tree.h /usr/local/include/

# 라이브러리 파일 복사
cp lib/libsegtree.a /usr/local/lib/

# 컴파일시 링크
gcc -o myprogram myprogram.c -lsegtree
```

## 🎯 실제 사용 사례

### 1. 배열 범위 쿼리

```c
// 주식 가격 배열에서 특정 기간의 최고가/최저가 찾기
int prices[] = {100, 80, 60, 70, 60, 75, 85};
segment_tree_t *max_st = segtree_create(prices, 7, SEGTREE_MAX);
segment_tree_t *min_st = segtree_create(prices, 7, SEGTREE_MIN);

int max_price, min_price;
segtree_query(max_st, 1, 5, &max_price);  // 1~5일간 최고가
segtree_query(min_st, 1, 5, &min_price);  // 1~5일간 최저가
```

### 2. 2D 범위 쿼리

```c
// 2D 배열을 1D로 변환하여 사용
int get_2d_index(int row, int col, int cols) {
    return row * cols + col;
}

// 행별 구간 합 계산
for (int row = 0; row < rows; row++) {
    int start = get_2d_index(row, col1, cols);
    int end = get_2d_index(row, col2, cols);
    segtree_query(st, start, end, &row_sum);
}
```

### 3. 동적 배열 처리

```c
// 배열이 변경될 때 트리 재구성
int new_arr[] = {5, 10, 15, 20};
segtree_rebuild(st, new_arr, 4);
```

## ⚠️ 제한사항 및 주의사항

### 제한사항
- 최대 배열 크기: 1,000,000 요소
- 정수형 데이터만 지원
- 단일 스레드 환경용

### 주의사항
- 지연 전파를 사용하지 않는 트리에서는 `segtree_update_range()` 호출 불가
- 범위는 0부터 시작하는 인덱스 사용
- 메모리 해제를 위해 반드시 `segtree_destroy()` 호출

## 🔍 문제 해결

### 자주 묻는 질문

**Q: 세그먼트 트리가 생성되지 않습니다.**
```
A: 다음을 확인하세요:
   - 배열이 NULL이 아닌가?
   - 배열 크기가 0보다 큰가?
   - 배열 크기가 SEGTREE_MAX_SIZE를 초과하지 않는가?
```

**Q: 쿼리 결과가 예상과 다릅니다.**
```
A: 다음을 확인하세요:
   - 범위 인덱스가 올바른가? (0부터 시작)
   - 올바른 연산 타입을 사용했는가?
   - 업데이트 후 지연 전파가 제대로 처리되었는가?
```

**Q: 메모리 누수가 발생합니다.**
```
A: segtree_destroy()를 호출했는지 확인하세요.
   Valgrind로 메모리 검사: make valgrind-tests
```

### 디버깅

```c
// 트리 구조 출력
segtree_print(st);

// 트리 유효성 검증
if (!segtree_validate(st)) {
    printf("트리 구조에 오류가 있습니다!\n");
}

// 성능 통계 확인
segtree_stats_t stats = segtree_get_stats(st);
printf("쿼리 수: %d, 평균 시간: %.6f초\n",
       stats.query_count,
       stats.total_query_time / stats.query_count);
```

## 📖 추가 문서

- [아키텍처 설계 문서](docs/segment_tree_architecture.md)
- [구현 상세 논문](docs/segment_tree_implementation_paper.md)

## 🤝 기여하기

1. Fork 저장소
2. 기능 브랜치 생성 (`git checkout -b feature/amazing-feature`)
3. 변경사항 커밋 (`git commit -m 'Add amazing feature'`)
4. 브랜치에 푸시 (`git push origin feature/amazing-feature`)
5. Pull Request 생성

### 개발 환경 설정

```bash
# 저장소 클론
git clone <repository-url>
cd segment-tree

# 개발용 빌드
make debug

# 테스트 실행
make run-tests

# 코드 스타일 검사
make lint  # (구현 예정)
```

## 📄 라이선스

이 프로젝트는 MIT 라이선스 하에 배포됩니다. 자세한 내용은 `LICENSE` 파일을 참조하세요.

## ✨ 버전 히스토리

- **v1.0.0** - 초기 릴리즈
  - 기본 Segment Tree 구현
  - 합, 최솟값, 최댓값 연산 지원
  - 지연 전파 기능
  - 성능 통계 및 디버깅 도구

## 📞 연락처

프로젝트와 관련된 질문이나 제안사항이 있으시면 이슈를 생성해 주세요.

---

**Segment Tree** - 효율적인 범위 쿼리를 위한 고성능 C 라이브러리 🚀