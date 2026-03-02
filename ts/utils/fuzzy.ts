export function fuzzyScore(query: string, str: string) {
    let score = 0;
    let lastIndex = -1;

    for (const char of query) {
        const index = str.indexOf(char, lastIndex + 1);
        if (index === -1) return -1; // no match
        score += 100 - (index - lastIndex); // closer = higher score
        lastIndex = index;
    }

    return score;
}

export function fuzzySort(
    arr: string[],
    query: string,
    keyFn?: (x: string) => string
): string[];

export function fuzzySort<T>(
    arr: T[],
    query: string,
    keyFn: (x: T) => string
): T[];

export function fuzzySort<T = any>(
    arr: T[],
    query: string,
    keyFn: (x: any) => string = (x: any) => x
): T[] {
    if (!query) return arr;

    const q = query.toLowerCase();

    const result = [];
    for (const item of arr) {
        const s = fuzzyScore(q, keyFn(item).toLowerCase());
        if (s > 0) result.push({ item, score: s });
    }

    result.sort((a, b) => b.score - a.score);

    return result.map(r => r.item);
}
