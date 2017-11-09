# python3
# utf-8
import copy


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
    ord___pair_hash = [(ord(x[0]) + hash___int(x[1]) * 256) for x in ord___sym__hash]
    return (list___hash(ord___pair_hash, PRIME1),
        list___hash(ord___pair_hash, PRIME2)
    )


class FSA:
    def __init__(self, words=[]):
        self.hash___node = dict()
        self.root_hash = (0, 0)
        self.hash___node[self.root_hash] = {}
        for word in words:
            self.add_word(word)

    def add_word(self, word):
        word += '~'
        curr_node_hash = self.root_hash
        path_ord___node_hash = []
        for stop_idx, letter in enumerate(word):
            path_ord___node_hash.append(curr_node_hash)
            next_node_hash = self.hash___node[curr_node_hash].get(letter)
            if next_node_hash is None:
                break
            curr_node_hash = next_node_hash
        curr_suffix_hash = TERMINAL_HASH
        for letter in word[-1: stop_idx: -1]:
            curr_node = {letter: curr_suffix_hash}
            curr_suffix_hash = node___hash(curr_node)
            self.hash___node[curr_suffix_hash] = curr_node
        for letter, curr_node_hash in zip(word[stop_idx:: -1], path_ord___node_hash[::-1]):
            curr_node = copy.deepcopy(self.hash___node[curr_node_hash])
            curr_node[letter] = curr_suffix_hash
            curr_suffix_hash = node___hash(curr_node)
            self.hash___node[curr_suffix_hash] = curr_node
        self.root_hash = node___hash(self.hash___node[curr_suffix_hash])

    def check_word(self, word):
        word += '~'
        curr_node_hash = self.root_hash
        for letter in word:
            curr_node_hash = self.hash___node[curr_node_hash].get(letter)
            if curr_node_hash is None:
                return False
        return True

# tests
import itertools
from time import sleep

fsa = FSA()
for product in itertools.product('abc', repeat=3):
    word = ''.join(product)
    if fsa.check_word(word):
        print(word)
        break
    fsa.add_word(word)
    if not fsa.check_word(word):
        print(word)
        break

for product in itertools.product('abc', repeat=7):
    word = ''.join(product)
    if fsa.check_word(word):
        print(word)
        break
    fsa.add_word(word)
    if not fsa.check_word(word):
        print(word)
        break
