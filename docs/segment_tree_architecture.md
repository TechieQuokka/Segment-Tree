# Segment Tree 아키텍처 설계 문서

## 1. 개요

### 1.1 목적
본 문서는 C언어로 구현할 Segment Tree(구간 트리)의 아키텍처 설계를 다룹니다. Segment Tree는 배열의 특정 구간에 대한 쿼리(합, 최댓값, 최솟값 등)를 효율적으로 처리하기 위한 자료구조입니다.

### 1.2 범위
- 기본 Segment Tree 구조 설계
- 메모리 관리 전략
- API 인터페이스 설계
- 확장 가능한 구조 설계

## 2. 시스템 아키텍처

### 2.1 전체 구조

```
segment_tree/
├── include/
│   └── segment_tree.h      # 공용 헤더 파일
├── src/
│   ├── segment_tree.c      # 핵심 구현
│   ├── operations.c        # 연산 함수들
│   └── utils.c            # 유틸리티 함수들
├── tests/
│   ├── test_basic.c       # 기본 기능 테스트
│   ├── test_performance.c  # 성능 테스트
│   └── test_edge_cases.c  # 경계 조건 테스트
├── examples/
│   ├── example_sum.c      # 구간 합 예제
│   ├── example_min_max.c  # 최솟값/최댓값 예제
│   └── example_lazy.c     # 지연 전파 예제
├── docs/
│   ├── architecture.md    # 본 문서
│   ├── api_reference.md   # API 참조
│   └── implementation.md  # 구현 상세
└── Makefile
```

### 2.2 핵심 컴포넌트

#### 2.2.1 데이터 구조 (segment_tree.h)

```c
// 노드 타입 정의
typedef enum {
    SEGTREE_SUM,      // 구간 합
    SEGTREE_MIN,      // 구간 최솟값
    SEGTREE_MAX,      // 구간 최댓값
    SEGTREE_CUSTOM    // 사용자 정의 연산
} segtree_operation_t;

// 메인 구조체
typedef struct segment_tree {
    int *tree;                    // 트리 배열
    int *lazy;                    // 지연 전파 배열
    int size;                     // 원본 배열 크기
    int tree_size;               // 트리 배열 크기
    segtree_operation_t op_type; // 연산 타입

    // 함수 포인터들
    int (*combine)(int a, int b);              // 두 값을 결합하는 함수
    void (*update_lazy)(struct segment_tree *st, int node, int start, int end, int val);
    void (*propagate)(struct segment_tree *st, int node, int start, int end);

    // 메타데이터
    int identity;                // 항등원 (합: 0, 최솟값: INF, 최댓값: -INF)
    bool lazy_enabled;           // 지연 전파 사용 여부
} segment_tree_t;
```

#### 2.2.2 메모리 관리 전략

1. **동적 할당**: 트리 크기에 따른 메모리 효율성
2. **메모리 풀**: 빈번한 할당/해제 최적화 (선택적)
3. **오류 처리**: 메모리 부족 상황에 대한 안전한 처리

#### 2.2.3 API 계층 구조

```
Application Layer
     |
API Interface Layer (segment_tree.h)
     |
Core Implementation Layer
     |
Memory Management Layer
     |
Platform Abstraction Layer
```

## 3. 세부 설계

### 3.1 초기화 및 생성

```c
// 기본 생성자
segment_tree_t* segtree_create(int *arr, int n, segtree_operation_t op);

// 사용자 정의 연산으로 생성
segment_tree_t* segtree_create_custom(int *arr, int n,
                                     int (*combine)(int, int),
                                     int identity);

// 지연 전파 지원 생성자
segment_tree_t* segtree_create_lazy(int *arr, int n, segtree_operation_t op);

// 소멸자
void segtree_destroy(segment_tree_t *st);
```

### 3.2 핵심 연산

```c
// 구간 쿼리
int segtree_query(segment_tree_t *st, int left, int right);

// 점 업데이트
void segtree_update_point(segment_tree_t *st, int index, int value);

// 구간 업데이트 (지연 전파)
void segtree_update_range(segment_tree_t *st, int left, int right, int value);

// 배열 재구성
void segtree_rebuild(segment_tree_t *st, int *new_arr, int new_size);
```

### 3.3 유틸리티 함수

```c
// 트리 출력 (디버깅용)
void segtree_print(segment_tree_t *st);

// 트리 검증
bool segtree_validate(segment_tree_t *st);

// 통계 정보
typedef struct {
    int query_count;
    int update_count;
    double avg_query_time;
    double avg_update_time;
} segtree_stats_t;

segtree_stats_t segtree_get_stats(segment_tree_t *st);
```

## 4. 성능 고려사항

### 4.1 시간 복잡도
- 구축: O(n)
- 쿼리: O(log n)
- 점 업데이트: O(log n)
- 구간 업데이트: O(log n) with lazy propagation

### 4.2 공간 복잡도
- 기본: O(4n) = O(n)
- 지연 전파 포함: O(8n) = O(n)

### 4.3 최적화 전략

1. **캐시 지역성 개선**
   - 배열 기반 구현으로 연속된 메모리 접근
   - 노드 번호 매핑 최적화

2. **분기 예측 최적화**
   - 조건문 최소화
   - 함수 포인터 vs 직접 구현 trade-off

3. **메모리 접근 패턴 최적화**
   - 프리페칭 힌트 제공
   - 메모리 정렬 고려

## 5. 확장성 설계

### 5.1 모듈화
각 기능을 독립적인 모듈로 분리하여 유지보수성 향상

### 5.2 플러그인 아키텍처
사용자 정의 연산을 위한 확장 가능한 구조

### 5.3 멀티스레드 지원 (미래 확장)
```c
typedef struct {
    segment_tree_t *st;
    pthread_mutex_t *locks;  // 노드별 락
    int num_threads;
} threadsafe_segtree_t;
```

## 6. 오류 처리 및 검증

### 6.1 오류 코드 정의
```c
typedef enum {
    SEGTREE_OK = 0,
    SEGTREE_ERROR_NULL_POINTER,
    SEGTREE_ERROR_INVALID_RANGE,
    SEGTREE_ERROR_MEMORY_ALLOCATION,
    SEGTREE_ERROR_INVALID_OPERATION
} segtree_error_t;
```

### 6.2 입력 검증
- 널 포인터 검사
- 배열 인덱스 범위 검사
- 구간 유효성 검사

### 6.3 디버깅 지원
```c
#ifdef DEBUG
    #define SEGTREE_ASSERT(condition, message) \
        do { if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s in %s:%d\n", \
                   message, __FILE__, __LINE__); \
            abort(); \
        } } while(0)
#else
    #define SEGTREE_ASSERT(condition, message)
#endif
```

## 7. 테스트 전략

### 7.1 단위 테스트
- 각 함수별 독립적 테스트
- 경계 조건 테스트
- 오류 상황 테스트

### 7.2 통합 테스트
- 전체 워크플로우 테스트
- 다양한 데이터 크기로 테스트

### 7.3 성능 테스트
- 시간 복잡도 검증
- 메모리 사용량 측정
- 실제 사용 시나리오 벤치마크

## 8. 빌드 시스템

### 8.1 Makefile 구조
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUG_FLAGS = -g -DDEBUG
TEST_FLAGS = -lgtest

SRCDIR = src
INCDIR = include
TESTDIR = tests
OBJDIR = obj

# 타겟들
all: lib test examples
lib: libsegtree.a
test: test_runner
examples: example_sum example_min_max
```

### 8.2 설정 옵션
- 컴파일 타임 설정
- 디버그 모드 지원
- 성능 최적화 레벨 선택

## 9. 문서화 전략

### 9.1 API 문서
- 함수별 상세 설명
- 사용 예제
- 성능 특성

### 9.2 구현 가이드
- 내부 알고리즘 설명
- 최적화 기법
- 확장 방법

### 9.3 사용자 가이드
- 빠른 시작 가이드
- 일반적인 사용 패턴
- 문제 해결 가이드

## 10. 결론

본 아키텍처는 확장 가능하고 효율적인 Segment Tree 구현을 위한 견고한 기반을 제공합니다. 모듈화된 설계를 통해 유지보수성을 높이고, 성능 최적화를 통해 실용적인 사용을 가능하게 합니다.