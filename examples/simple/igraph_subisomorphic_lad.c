/* -*- mode: C -*-  */
/*
   IGraph library.
   Copyright (C) 2012  Gabor Csardi <csardi.gabor@gmail.com>
   334 Harvard street, Cambridge, MA 02139 USA

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA

*/

#include <igraph.h>

void print_maps(igraph_vector_int_t *map, igraph_vector_ptr_t *maps) {
    igraph_integer_t n, i;
    igraph_vector_int_print(map);
    n = igraph_vector_ptr_size(maps);
    for (i = 0; i < n; i++) {
        igraph_vector_int_t *v = VECTOR(*maps)[i];
        igraph_vector_int_print(v);
        igraph_vector_int_destroy(v);
        igraph_free(v);
    }
}

/* This test counts motifs using LAD and compares the results with
 * the RANDESU motif finder */
void test_motifs() {
    igraph_t graph;
    igraph_vector_t randesu_counts, lad_counts;
    igraph_vector_t cut_prob;
    igraph_integer_t i, n;
    igraph_bool_t equal;
    igraph_integer_t vcount;

    igraph_rng_seed(igraph_rng_default(), 42);

    igraph_erdos_renyi_game_gnm(&graph, 40, 400, /* directed = */ 1, /* loops = */ 0);
    vcount = igraph_vcount(&graph);

    /* 3-motifs */

    n = 16; /* there are 16 size-3 directed graphs */

    igraph_vector_init(&lad_counts, n);

    for (i = 0; i < n; i++) {
        igraph_t pattern;
        igraph_vector_ptr_t maps;
        igraph_integer_t nAutomorphisms;

        igraph_isoclass_create(&pattern, 3, i, /* directed = */ 1);
        igraph_vector_ptr_init(&maps, 0);

        igraph_subisomorphic_lad(&pattern, &graph, NULL, NULL, NULL, &maps, /* induced = */ 1, 0);

        igraph_count_subisomorphisms_vf2(&pattern, &pattern, NULL, NULL, NULL, NULL, &nAutomorphisms, NULL, NULL, NULL);

        VECTOR(lad_counts)[i] = igraph_vector_ptr_size(&maps) / nAutomorphisms;

        igraph_vector_ptr_destroy_all(&maps, igraph_vector_int_destroy);

        igraph_destroy(&pattern);
    }

    igraph_vector_init(&cut_prob, 3);
    igraph_vector_init(&randesu_counts, 0);
    igraph_motifs_randesu(&graph, &randesu_counts, 3, &cut_prob);

    equal = 1 /* true */;
    for (i = 0; i < n; i++) {
        if (igraph_is_nan(VECTOR(randesu_counts)[i])) {
            continue;
        }
        if (VECTOR(randesu_counts)[i] != VECTOR(lad_counts)[i]) {
            equal = 0;
            break;
        }
    }

    if (! equal) {
        printf("LAD directed 3-motif count does not agree with RANDESU.\n");
    }

    if (igraph_vector_sum(&lad_counts) != vcount * (vcount - 1) * (vcount - 2) / 6) {
        printf("Total 3-vertex directed subgraph count is incorrect.\n");
    }

    igraph_vector_destroy(&randesu_counts);
    igraph_vector_destroy(&lad_counts);
    igraph_vector_destroy(&cut_prob);

    /* 4-motifs */

    n = 218; /* there are 218 size-4 directed graphs */

    igraph_vector_init(&lad_counts, n);

    for (i = 0; i < n; i++) {
        igraph_t pattern;
        igraph_vector_ptr_t maps;
        igraph_integer_t nAutomorphisms;

        igraph_isoclass_create(&pattern, 4, i, /* directed = */ 1);
        igraph_vector_ptr_init(&maps, 0);

        igraph_subisomorphic_lad(&pattern, &graph, NULL, NULL, NULL, &maps, /* induced = */ 1, 0);

        igraph_count_subisomorphisms_vf2(&pattern, &pattern, NULL, NULL, NULL, NULL, &nAutomorphisms, NULL, NULL, NULL);

        VECTOR(lad_counts)[i] = igraph_vector_ptr_size(&maps) / nAutomorphisms;

        igraph_vector_ptr_destroy_all(&maps, igraph_vector_int_destroy);

        igraph_destroy(&pattern);
    }

    igraph_vector_init(&cut_prob, 4);
    igraph_vector_init(&randesu_counts, 0);
    igraph_motifs_randesu(&graph, &randesu_counts, 4, &cut_prob);

    equal = 1 /* true */;
    for (i = 0; i < n; i++) {
        if (igraph_is_nan(VECTOR(randesu_counts)[i])) {
            continue;
        }
        if (VECTOR(randesu_counts)[i] != VECTOR(lad_counts)[i]) {
            equal = 0;
            break;
        }
    }

    if (! equal) {
        printf("LAD directed 4-motif count does not agree with RANDESU.\n");
    }

    if (igraph_vector_sum(&lad_counts) != vcount * (vcount - 1) * (vcount - 2) * (vcount - 3) / 24) {
        printf("Total 4-vertex directed subgraph count is incorrect.\n");
    }

    igraph_vector_destroy(&randesu_counts);
    igraph_vector_destroy(&lad_counts);
    igraph_vector_destroy(&cut_prob);

    igraph_destroy(&graph);
}


void test_motifs_undirected() {
    igraph_t graph;
    igraph_vector_t randesu_counts, lad_counts;
    igraph_vector_t cut_prob;
    igraph_integer_t i, n;
    igraph_bool_t equal;
    igraph_integer_t vcount;

    igraph_rng_seed(igraph_rng_default(), 42);

    igraph_erdos_renyi_game_gnm(&graph, 20, 100, /* directed = */ 0, /* loops = */ 0);
    vcount = igraph_vcount(&graph);

    /* 3-motifs */

    n = 4; /* there are 4 size-3 undirected graphs */

    igraph_vector_init(&lad_counts, n);

    for (i = 0; i < n; i++) {
        igraph_t pattern;
        igraph_vector_ptr_t maps;
        igraph_integer_t nAutomorphisms;

        igraph_isoclass_create(&pattern, 3, i, /* directed = */ 0);
        igraph_vector_ptr_init(&maps, 0);

        igraph_subisomorphic_lad(&pattern, &graph, NULL, NULL, NULL, &maps, /* induced = */ 1, 0);

        igraph_count_subisomorphisms_vf2(&pattern, &pattern, NULL, NULL, NULL, NULL, &nAutomorphisms, NULL, NULL, NULL);

        VECTOR(lad_counts)[i] = igraph_vector_ptr_size(&maps) / nAutomorphisms;

        igraph_vector_ptr_destroy_all(&maps, igraph_vector_int_destroy);

        igraph_destroy(&pattern);
    }

    igraph_vector_init(&cut_prob, 3);
    igraph_vector_init(&randesu_counts, 0);
    igraph_motifs_randesu(&graph, &randesu_counts, 3, &cut_prob);

    equal = 1 /* true */;
    for (i = 0; i < n; i++) {
        if (igraph_is_nan(VECTOR(randesu_counts)[i])) {
            continue;
        }
        if (VECTOR(randesu_counts)[i] != VECTOR(lad_counts)[i]) {
            equal = 0;
            break;
        }
    }

    if (! equal) {
        printf("LAD undirected 3-motif count does not agree with RANDESU.\n");
    }

    if (igraph_vector_sum(&lad_counts) != vcount * (vcount - 1) * (vcount - 2) / 6) {
        printf("Total 3-vertex undirected subgraph count is incorrect.\n");
    }

    igraph_vector_destroy(&randesu_counts);
    igraph_vector_destroy(&lad_counts);
    igraph_vector_destroy(&cut_prob);

    /* 4-motifs */

    n = 11; /* there are 11 size-4 undirected graphs */

    igraph_vector_init(&lad_counts, n);

    for (i = 0; i < n; i++) {
        igraph_t pattern;
        igraph_vector_ptr_t maps;
        igraph_integer_t nAutomorphisms;

        igraph_isoclass_create(&pattern, 4, i, /* directed = */ 0);
        igraph_vector_ptr_init(&maps, 0);

        igraph_subisomorphic_lad(&pattern, &graph, NULL, NULL, NULL, &maps, /* induced = */ 1, 0);

        igraph_count_subisomorphisms_vf2(&pattern, &pattern, NULL, NULL, NULL, NULL, &nAutomorphisms, NULL, NULL, NULL);

        VECTOR(lad_counts)[i] = igraph_vector_ptr_size(&maps) / nAutomorphisms;

        igraph_vector_ptr_destroy_all(&maps, igraph_vector_int_destroy);

        igraph_destroy(&pattern);
    }

    igraph_vector_init(&cut_prob, 4);
    igraph_vector_init(&randesu_counts, 0);
    igraph_motifs_randesu(&graph, &randesu_counts, 4, &cut_prob);

    equal = 1 /* true */;
    for (i = 0; i < n; i++) {
        if (igraph_is_nan(VECTOR(randesu_counts)[i])) {
            continue;
        }
        if (VECTOR(randesu_counts)[i] != VECTOR(lad_counts)[i]) {
            equal = 0;
            break;
        }
    }

    if (! equal) {
        printf("LAD undirected 4-motif count does not agree with RANDESU.\n");
    }

    if (igraph_vector_sum(&lad_counts) != vcount * (vcount - 1) * (vcount - 2) * (vcount - 3) / 24) {
        printf("Total 4-vertex undirected subgraph count is incorrect.\n");
    }

    igraph_vector_destroy(&randesu_counts);
    igraph_vector_destroy(&lad_counts);
    igraph_vector_destroy(&cut_prob);

    igraph_destroy(&graph);
}


int main() {
    igraph_t target, pattern;
    igraph_bool_t iso;
    igraph_vector_int_t map;
    igraph_vector_ptr_t maps;
    igraph_integer_t i, n;
    igraph_error_t result;
    int domainsvec[] = { 0, 2, 8, -1,
                         4, 5, 6, 7, -1,
                         1, 3, 5, 6, 7, 8, -1,
                         0, 2, 8, -1,
                         1, 3, 7, 8, -1, -2
                       };
    igraph_vector_ptr_t domains;
    igraph_vector_int_t *v = 0;

    igraph_small(&target, 9, IGRAPH_UNDIRECTED,
                 0, 1, 0, 4, 0, 6,
                 1, 0, 1, 4, 1, 2,
                 2, 1, 2, 3,
                 3, 2, 3, 4, 3, 5, 3, 7, 3, 8,
                 4, 0, 4, 1, 4, 3, 4, 5, 4, 6,
                 5, 6, 5, 4, 5, 3, 5, 8,
                 6, 0, 6, 4, 6, 5,
                 7, 3, 7, 8,
                 8, 5, 8, 3, 8, 7,
                 -1);
    igraph_simplify(&target, /*multiple=*/ 1, /*loops=*/ 0, /*edge_comb=*/ 0);

    igraph_small(&pattern, 5, IGRAPH_UNDIRECTED,
                 0, 1, 0, 4,
                 1, 0, 1, 4, 1, 2,
                 2, 1, 2, 3,
                 3, 2, 3, 4,
                 4, 3, 4, 1, 4, 0,
                 -1);
    igraph_simplify(&pattern, /*multiple=*/ 1, /*loops=*/ 0, /*edge_comb=*/ 0);

    igraph_vector_int_init(&map, 0);
    igraph_vector_ptr_init(&maps, 0);

    igraph_subisomorphic_lad(&pattern, &target, /*domains=*/ 0, &iso, &map,
                             &maps, /*induced=*/ 0, /*time_limit=*/ 0);

    if (!iso) {
        return 1;
    }
    print_maps(&map, &maps);

    printf("---------\n");

    igraph_subisomorphic_lad(&pattern, &target, /*domains=*/ 0, &iso, &map,
                             &maps, /*induced=*/ 1, /*time_limit=*/ 0);

    if (!iso) {
        return 2;
    }
    print_maps(&map, &maps);

    printf("---------\n");

    igraph_vector_ptr_init(&domains, 0);
    i = 0;
    while (1) {
        if (domainsvec[i] == -2) {
            break;
        } else if (domainsvec[i] == -1) {
            igraph_vector_ptr_push_back(&domains, v);
            v = 0;
        } else {
            if (!v) {
                v = (igraph_vector_int_t *) malloc(sizeof(igraph_vector_int_t));
                igraph_vector_int_init(v, 0);
            }
            igraph_vector_int_push_back(v, domainsvec[i]);
        }
        i++;
    }

    igraph_subisomorphic_lad(&pattern, &target, &domains, &iso, &map, &maps,
                             /*induced=*/ 0, /*time_limit=*/ 0);

    if (!iso) {
        return 3;
    }
    print_maps(&map, &maps);

    n = igraph_vector_ptr_size(&domains);
    for (i = 0; i < n; i++) {
        igraph_vector_int_t *v = VECTOR(domains)[i];
        igraph_vector_int_destroy(v);
        free(v);
    }

    igraph_vector_ptr_destroy(&domains);
    igraph_vector_int_destroy(&map);
    igraph_vector_ptr_destroy(&maps);

    igraph_destroy(&pattern);
    igraph_destroy(&target);

    printf("---------\n");

    igraph_vector_int_init(&map, 0);
    igraph_vector_ptr_init(&maps, 0);

    igraph_small(&target, 9, IGRAPH_UNDIRECTED,
                 0, 1, 0, 4, 0, 6,
                 1, 0, 1, 4, 1, 2,
                 2, 1, 2, 3,
                 3, 2, 3, 4, 3, 5, 3, 7, 3, 8,
                 4, 0, 4, 1, 4, 3, 4, 5, 4, 6,
                 5, 6, 5, 4, 5, 3, 5, 8,
                 6, 0, 6, 4, 6, 5,
                 7, 3, 7, 8,
                 8, 5, 8, 3, 8, 7,
                 -1);
    igraph_simplify(&target, /*multiple=*/ 1, /*loops=*/ 0, /*edge_comb=*/ 0);

    igraph_small(&pattern, 0, IGRAPH_DIRECTED, -1);
    igraph_set_error_handler(igraph_error_handler_ignore);
    result = igraph_subisomorphic_lad(&pattern, &target, /*domains=*/ 0,
                                      &iso, &map, &maps, /*induced=*/ 0,
                                      /*time_limit=*/ 0);
    igraph_set_error_handler(igraph_error_handler_abort);
    if (result != IGRAPH_EINVAL) {
        return 4;
    }
    igraph_destroy(&pattern);

    igraph_small(&pattern, 0, IGRAPH_UNDIRECTED, -1);
    igraph_subisomorphic_lad(&pattern, &target, /*domains=*/ 0, &iso, &map, &maps,
                             /*induced=*/ 0, /*time_limit=*/ 0);
    if (!iso) {
        return 5;
    }
    if (igraph_vector_int_size(&map) != 0) {
        return 6;
    }
    if (igraph_vector_ptr_size(&maps) != 0) {
        return 7;
    }

    igraph_destroy(&pattern);
    igraph_destroy(&target);

    igraph_vector_int_destroy(&map);
    igraph_vector_ptr_destroy(&maps);

    test_motifs();
    test_motifs_undirected();

    return 0;
}
