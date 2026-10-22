/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nychan <nychan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 16:17:52 by nychan            #+#    #+#             */
/*   Updated: 2026/10/13 12:12:03 by nychan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/**
 * =========================================================================
 * FUNCTION: merge
 * =========================================================================
 * OVERVIEW:
 *  Merges two adjacent sorted subarrays (left..mid and mid+1..right) back
 *  into 'arr' in ascending order while tallying split inversions. Each time
 *  an element from the right subarray is picked before an element in the
 *  left subarray, it indicates that all remaining elements in the left half
 *  are strictly greater than that right element.
 *
 * EXECUTION SCHEMA:
 *
 *   Copy left slice to tmp: tmp[left..mid] = arr[left..mid]
 *            │
 *            ▼
 *   Compare tmp[left_i] vs arr[mid_i]:
 *        ├── tmp[left_i] <= arr[mid_i]
 *        │     └── arr[left++] = tmp[left_i++]
 *        │
 *        └── tmp[left_i] > arr[mid_i]
 *              ├── arr[left++] = arr[mid_i++]
 *              └── inv += (mid - left_i + 1)  [Count split inversions]
 *            │
 *            ▼
 *   Copy remaining tmp elements back into arr
 *   Return inv
 *
 * PARAMETERS:
 *  - arr {int *}: Base array containing subarrays to merge.
 *  - tmp {int *}: Scratch buffer used to store the left partition.
 *  - left {int}: Starting index of left subarray.
 *  - right {int}: Ending index of right subarray.
 *  - mid {int}: Division boundary index between subarrays.
 *
 * RETURNS:
 *  - {long long}: Number of inversions detected between the two halves.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(R - L + 1) linear in slice size.
 *  - Space Complexity: O(1) auxiliary space (uses preallocated tmp).
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Assumes input subarrays are already sorted internally.
 *  - Right subarray elements left over do not need copying since they
 *    already occupy their target positions in 'arr'.
 * =========================================================================
 */
static long long	merge(t_nums *nums, int left, int right, int mid)
{
	int			left_i;
	int			mid_i;
	long long	inv;

	inv = 0;
	left_i = left - 1;
	mid_i = mid + 1;
	while (++left_i <= mid)
		nums->tmp[left_i] = nums->arr[left_i];
	left_i = left;
	while (left_i <= mid && mid_i <= right)
	{
		if (nums->tmp[left_i] <= nums->arr[mid_i])
			nums->arr[left++] = nums->tmp[left_i++];
		else
		{
			nums->arr[left++] = nums->arr[mid_i++];
			inv += (mid - left_i + 1);
		}
	}
	while (left_i <= mid)
		nums->arr[left++] = nums->tmp[left_i++];
	return (inv);
}

/**
 * =========================================================================
 * FUNCTION: sort
 * =========================================================================
 * OVERVIEW:
 *  Recursive divide-and-conquer routine for merge sort and inversion
 *  counting. Recursively splits the array slice in half, accumulates
 *  inversions from both child halves, and adds cross-boundary inversions
 *  found during the merge step.
 *
 * EXECUTION SCHEMA:
 *
 *   left >= right? ──[Yes]──> Return 0 (Base case: 0 or 1 element)
 *        │ [No]
 *        ▼
 *   mid = left + (right - left) / 2
 *        │
 *        ├── inv  = sort(arr, tmp, left, mid)        [Left half]
 *        ├── inv += sort(arr, tmp, mid + 1, right)  [Right half]
 *        └── inv += merge(arr, tmp, left, right, mid)[Merge halves]
 *        │
 *        ▼
 *   Return inv
 *
 * PARAMETERS:
 *  - arr {int *}: Subarray slice being sorted.
 *  - tmp {int *}: Scratch buffer allocated for merging operations.
 *  - left {int}: Lower bound index.
 *  - right {int}: Upper bound index.
 *
 * RETURNS:
 *  - {long long}: Total count of inversions present in arr[left..right].
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N log N), where N = right - left + 1.
 *  - Space Complexity: O(log N) recursive call stack depth.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Uses mid = left + (right - left) / 2 to guard against int overflow.
 *  - Leaves the target slice in strictly ascending order upon return.
 * =========================================================================
 */
static long long	sort(t_nums *nums, int left, int right)
{
	long long	inv;
	int			mid;

	inv = 0;
	mid = left + (right - left) / 2;
	if (left >= right)
		return (0);
	inv += sort(nums, left, mid);
	inv += sort(nums, mid + 1, right);
	inv += merge(nums, left, right, mid);
	return (inv);
}

/**
 * =========================================================================
 * FUNCTION: merge_sort
 * =========================================================================
 * OVERVIEW:
 *  Public interface for merge sort inversion counting. Allocates scratch
 *  memory via s_alloc and coordinates recursive merge-sorting across the
 *  entire array slice [0..size - 1].
 *
 * EXECUTION SCHEMA:
 *
 *   size < 2? ────────────[Yes]───> Return 0 (Trivially sorted)
 *        │ [No]
 *        ▼
 *   tmp = s_alloc(size * sizeof(int))
 *        │
 *        ├── [NULL] ──────────────> g_free() + exit(1) (OOM)
 *        │
 *        ▼ [Valid buffer]
 *   sort(arr, tmp, 0, size - 1) ──> Sort arr and count inversions
 *
 * PARAMETERS:
 *  - arr {int *}: Integer array to be sorted and measured.
 *  - size {int}: Total element count in the array.
 *
 * RETURNS:
 *  - {long long}: Total inversion count within the input array.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N log N), where N is array size.
 *  - Space Complexity: O(N) heap memory tracked by the garbage collector.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Directly mutates 'arr', leaving it completely sorted in ascending
 *    order (serving as config->nums.arr sorted reference for strategies).
 *  - Returns 0 safely without allocating memory if size < 2.
 * =========================================================================
 */
long long	merge_sort(t_nums *nums)
{
	if (nums->size < 2)
		return (0);
	nums->tmp = s_alloc(nums->size * sizeof(int));
	if (!nums->tmp)
		exit ((g_free(), 1));
	return (sort(nums, 0, nums->size - 1));
}

/**
 * =========================================================================
 * FUNCTION: calculate_disorder
 * =========================================================================
 * OVERVIEW:
 *  Calculates the normalized disorder score (Kendall tau distance fraction)
 *  of the input permutation: inversions / total_possible_pairs. Also sorts
 *  config->nums.arr in place and triggers pivot optimization for complex
 *  strategy partitioning.
 *
 * EXECUTION SCHEMA:
 *
 *   nums.arr (Initial unsorted state)
 *        │
 *        ▼
 *   inv = merge_sort(nums.arr, size) ──> Sorts arr & counts inversions
 *        │
 *        ├── inv <= 0 || size < 2? ──[Yes]──> Return 0.0 (Already sorted)
 *        │
 *        ▼ [Positive inversions]
 *   total_pair = (size * (size - 1)) / 2
 *   calculate_pivot(config)           ──> Evaluate partition ratios
 *        │
 *        ▼
 *   Return (float)inv / total_pair
 *
 * PARAMETERS:
 *  - config {t_config *}: Program context containing stack data and state.
 *
 * RETURNS:
 *  - {float}: Normalized disorder ratio in range [0.0, 1.0], where 0.0 is
 *             completely sorted and 1.0 is completely reversed.
 *
 * COMPLEXITY:
 *  - Time Complexity:  O(N log N) dominated by merge_sort and pivot search.
 *  - Space Complexity: O(N) heap allocations for scratch buffers.
 *
 * EDGE CASES & ASSUMPTIONS:
 *  - Uses 64-bit integer arithmetic (long long) for total_pair to prevent
 *    overflow when size is large (e.g., size > 46,340).
 *  - Side Effect: Permanently sorts config->nums.arr into ascending order.
 * =========================================================================
 */
float	calculate_disorder(t_config *config)
{
	long long	total_pair;
	long long	inv;
	int			size;
	t_nums		nums;

	nums = config->nums;
	size = nums.size;
	inv = merge_sort(&config->nums);
	if (inv <= 0 || size < 2)
		return (0);
	total_pair = ((long long)size * (size - 1)) / 2;
	calculate_pivot(config);
	return ((float)inv / total_pair);
}
