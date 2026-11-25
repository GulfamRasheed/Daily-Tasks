from typing import List


def merge(intervals: List[List[int]]) -> List[List[int]]:
    if not intervals:
        return []

    intervals.sort(key=lambda x: x[0])
    merged = [intervals[0][:]]

    for start, end in intervals[1:]:
        last = merged[-1]
        if start > last[1]:
            merged.append([start, end])
        else:
            last[1] = max(last[1], end)

    return merged


if __name__ == "__main__":
    print(merge([[1, 3], [2, 6], [8, 10], [15, 18]]))
    print(merge([[1, 4], [4, 5]]))
    print(merge([[1, 4], [0, 4]]))
    print(merge([]))
