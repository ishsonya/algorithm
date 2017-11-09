# python3
# utf-8

from itertools import product
import copy
from collections import Counter

MAX_DL_DIST = 2

MAX_HASH = 10**9 + 7
PRIME1 = 1000000207
PRIME2 = 1000001269
MAX_PAIR_HASH = MAX_HASH * 256
TERMINAL_HASH = (1, 0)

def list___hash(idx___value, mod):
    ans = 0
    for value in idx___value:
        ans *= MAX_PAIR_HASH
        ans += value
        ans %= mod
    return ans

def hash___int(hash):
    return (hash[1] * PRIME1 + hash[0])

def node___hash(node):
    # node is a dict {[a-z]:int}
    child___node_hash = node
    ord___sym__hash = sorted(list(child___node_hash.items()))
    ord___pair_hash = [(x[0] + hash___int(x[1]) * 256) for x in ord___sym__hash]
    return (list___hash(ord___pair_hash, PRIME1),
        list___hash(ord___pair_hash, PRIME2)
    )
def state___int(state):
    prev_loc_idx___dist, pprev_loc_idx___dist, prev_loc_idx___is_eq = state
    ans = 0
    for dist in prev_loc_idx___dist:
        ans *= MAX_DL_DIST + 2
        ans += dist
    for dist in pprev_loc_idx___dist:
        ans *= MAX_DL_DIST + 2
        ans += dist
    for is_eq in prev_loc_idx___is_eq:
        ans *= 2
        ans += int(is_eq)
    return ans

def mask___int(mask):
    ans = 0
    for is_eq in mask:
        ans *= 2
        ans += int(is_eq)
    return ans

def drow__dcol_from_curr_start___value(drow, dcol, curr_loc_idx___dist, prev_loc_idx___dist, pprev_loc_idx___dist):
    if drow == 0:
        if 0 <= dcol < 2 * MAX_DL_DIST + 1:
            return curr_loc_idx___dist[dcol]
        else:
            return (MAX_DL_DIST + 1)
    elif drow == -1:
        prev_loc_col = dcol + 1
        if 0 <= prev_loc_col < MAX_DL_DIST * 2 + 1:
            return prev_loc_idx___dist[prev_loc_col]
        else:
            return (MAX_DL_DIST + 1)
    elif drow == -2:
        pprev_loc_col = dcol + 1
        if 0 <= pprev_loc_col < MAX_DL_DIST * 2 - 1:
            return pprev_loc_idx___dist[pprev_loc_col]
        else:
            return (MAX_DL_DIST + 1)
    else:
        raise Exception('INVALID DROW')

def drow_dcol_from_curr_start___is_eq(drow, dcol, prev_loc_idx___is_eq, loc_idx___is_eq):
    if drow == 0:
        if 0 <= dcol < 2 * MAX_DL_DIST + 1:
            return loc_idx___is_eq[dcol]
        else:
            return False
    elif drow == -1:
        prev_dcol = dcol - 1
        # print(prev_dcol)
        # print(prev_loc_idx___is_eq)
        if 0 <= prev_dcol < 2 * MAX_DL_DIST - 1:
            return prev_loc_idx___is_eq[prev_dcol]
        else:
            return False
    else:
        raise Exception('INVALID DROW')

def state___good_state(state):
    prev_loc_idx___dist, pprev_loc_idx___dist, prev_loc_idx___is_eq = state
    stats = ([MAX_DL_DIST + 1] * (2 * MAX_DL_DIST + 1),
        prev_loc_idx___dist,
        pprev_loc_idx___dist
    )
    eq_flag = True
    interesting_prev_row_dcols = range(2 * MAX_DL_DIST - 1)
    # if prev_loc_idx___dist[MAX_DL_DIST] == MAX_DL_DIST + 1:
    #     prev_loc_idx___dist = [3] * (2 * MAX_DL_DIST + 1)
    #     pprev_loc_idx___dist
    for dcol in interesting_prev_row_dcols:
        pred = drow__dcol_from_curr_start___value(-1, dcol, *stats) == drow__dcol_from_curr_start___value(-2, dcol - 1, *stats)
        eq_flag = eq_flag and pred
        if eq_flag:
            new_mask = [False for x in prev_loc_idx___is_eq]
            good_state = (prev_loc_idx___dist, pprev_loc_idx___dist, new_mask)
            return good_state
    else:
        return state

def state__mask___state(state, mask):
    prev_loc_idx___dist, pprev_loc_idx___dist, prev_loc_idx___is_eq = state
    loc_idx___is_eq = mask
    curr_dcol___dist = [MAX_DL_DIST + 1 for x in range(2 * MAX_DL_DIST + 1)]
    for dcol in range(2 * MAX_DL_DIST + 1):
        stats = (
            curr_dcol___dist,
            prev_loc_idx___dist,
            pprev_loc_idx___dist
        )
        curr_dcol___dist[dcol] = min(
            drow__dcol_from_curr_start___value(0, dcol - 1, *stats) + 1,
            curr_dcol___dist[dcol]
        )
        curr_dcol___dist[dcol] = min(
            drow__dcol_from_curr_start___value(-1, dcol, *stats) + 1,
            curr_dcol___dist[dcol]
        )
        # print('*')
        # print(prev_loc_idx___is_eq)
        # print(loc_idx___is_eq)
        masks = (prev_loc_idx___is_eq, loc_idx___is_eq)
        not_eq_flag = not drow_dcol_from_curr_start___is_eq(0, dcol, *masks)
        curr_dcol___dist[dcol] = min(
            drow__dcol_from_curr_start___value(-1, dcol - 1, *stats) + int(not_eq_flag),
            curr_dcol___dist[dcol]
        )
        eq_flag = (drow_dcol_from_curr_start___is_eq(0, dcol - 1, *masks)
            and drow_dcol_from_curr_start___is_eq(-1, dcol + 1, *masks)
        )
        if eq_flag:
            curr_dcol___dist[dcol] = min(
                drow__dcol_from_curr_start___value(-2, dcol - 1, *stats) + 1,
                curr_dcol___dist[dcol]
            )
    new_state = (curr_dcol___dist, prev_loc_idx___dist[1:-1], loc_idx___is_eq[2:])
    # print(state___good_state(new_state))
    # print('?')
    return state___good_state(new_state)

class DLAutomaton:
    def __init__(self):
        pprev = list(range(MAX_DL_DIST - 1, 0, -1)) + list(range(MAX_DL_DIST))
        prev = list(range(MAX_DL_DIST, 0, -1)) + list(range(MAX_DL_DIST + 1))
        mask = [False for x in range(2 * MAX_DL_DIST - 1)]
        self.root_state = (prev, pprev, mask)
        self.state_int___times_visited = Counter()
        self.hash___node = dict()

    def construct_minimal_automaton(self):
        curr_state = self.root_state
        curr_state_int = state___int(curr_state)
        self.state_int___times_visited[curr_state_int] += 1
        self.state_int___hash = {}
        self.hash___node = {}
        dfs_stack = []
        dfs_stack.append((curr_state, 'nowhere', 'init'))
        while dfs_stack:
            curr_state, where, why = dfs_stack.pop()
            curr_state_int = state___int(curr_state)
            # print(curr_state)
            if self.state_int___times_visited[curr_state_int] == 1:
                dfs_stack.append((curr_state, where, why))
                for mask in product([True, False], repeat=(2 * MAX_DL_DIST + 1)):
                    curr_new_state = state__mask___state(curr_state, mask)
                    curr_new_state_int = state___int(curr_new_state)
                    if self.state_int___times_visited[curr_new_state_int] > 0:
                        continue
                    self.state_int___times_visited[curr_new_state_int] += 1
                    dfs_stack.append((curr_new_state, curr_state, mask))
                self.state_int___times_visited[curr_state_int] += 1
            elif self.state_int___times_visited[curr_state_int] == 2:
                curr_node = {}
                print(curr_state, where, why)
                for mask in product([True, False], repeat=(2 * MAX_DL_DIST + 1)):
                    curr_new_state = state__mask___state(curr_state, mask)
                    curr_new_state_int = state___int(curr_new_state)
                    # print('****' + str(mask))
                    # print('****' + str(curr_new_state))
                    if curr_state_int == curr_new_state_int:
                        continue
                    curr_new_state_hash = self. ]
                    curr_node[mask___int(mask)] = curr_new_state_hash
                curr_hash = node___hash(curr_node)
                # print(curr_node)
                self.state_int___hash[curr_state_int] = curr_hash
                self.hash___node[curr_hash] = curr_node

a = DLAutomaton()
a.construct_minimal_automaton()
