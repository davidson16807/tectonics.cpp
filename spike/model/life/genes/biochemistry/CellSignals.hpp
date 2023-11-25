#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <model/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    struct CellSignals
    {
        // ribosome
        /*
        "cell_wall" indicates the ability to form a cell wall.
        Usage is disabled if a cell wall is present.
        This is most immediately useful for motion within unicellular organisms.
        */
        float cell_wall;
        /*
        "cytoskeleton" indicates whether signaling pathways exist to form and direct a cytoskeleton
        Usage is disabled if a cell wall is present.
        This is most immediately useful for motion within unicellular organisms.
        */
        float cytoskeleton;
        /*
        "lysin" indicates whether cells have the ability to manufacture lysing compounds,
        such as to cause other bacteria to lyse and release their nutrient rich interiors for consumption
        */
        float lysin;
        /*
        "stress_signaling" indicates whether cells are able to emit a unified chemical signal
        in response to a stress event such as nutrient depletion, oxidation, or radiation.
        */
        float stress_signaling;
        /*
        "phagocytosis" indicates whether signaling pathways exist for cells to envelop others.
        A cytoskeleton without cell wall is required to do so.
        If combined with "lysin", the enveloped cell may dissolve to release its contents for consumption.
        */
        float phagocytosis; 
        /*
        "pinocytosis" indicates whether signaling pathways exist for cells 
        to penetrate and suck the contents of other cells for consumption.
        */
        float pinocytosis; 
        /* 
        "natural competence" determines whether signaling pathways allow safe passage of foreign dna 
        into and throughout their cytoplasm when stressed.
        This is useful if asexual cells grow closely together in a typical setting.
        In a stressfull situation, some cells will have damaged dna and others may have lysed, releasing their own dna.
        If the cells are typically surrounded by clones, the only dna in the environment will likely be their own.
        If the cell accepts this dna, they will then have a redundant copy of their own dna,
        allowing them to continue producing essential proteins even if their original copy is damaged.
        If a common cause of stress is overpopulation then this will be especially useful.

        If combined with a form of autophagy, the model will interpret this as 
        the cell deliberately enveloping and lyse its clones in a stressful situation.
        This will be useful as an act of consolidating redundant copies of its genome 
        in the same place of manufacture. 
        It's not likely that cells will develop resistance to this kind of autophagy:
        if a gene arises that inhibits it, it will snuff itself out since the individual being eaten
        will itself be stressed in circumstances of autophagy and will benefit from consolidation as well.
        In this case, the gene will 
        However, this does not apply to forms of autophagy 
        that require releasing lysing compounds into the environment,
        since those lysing components would work just as well on the cell that releases them.
        This may not be the case if there is some dimorphism between cells,
        but for natural competence to initially develop in our scenario, 
        we must presume the cells are nondimorphic clones. 

        The interaction between natural competence and autophagy is an attempt by the model
        to provide a mechanism for the evolution of gamete fertilization.
        */
        float stressed_natural_competence;
        /*
        "stressed_autophagy" determines whether signaling pathways attempt endocytosis on clones of the cell when stressed.
        This may serve as a precursor to an immune response to damaged or cancerous cells in a colonial/multicellular organims,
        or it may serve as a precursor to syngamy: the fusion of gametes in sexual reproduction.
        If autophagy occurs then the cell enters a diploid state and requires meiotic division to remain viable,
        since increasing the amount of genetic material will eventually overwhelm the machinery that's meant to process it.
        */
        float stressed_autophagy;
        /*
        "homologous recombination" refers to the real world pairing of homologous genetic elements during recombination.
        It is very ubiquitous and occurs in all domains of life (Lenormand 2016)
        It could be useful as a way to ensure higher fidelity recombination (Wilkins 2009).
        In the model, "recombination_during_pairing" simulates homologous recombination
        by requiring recombination occurs only once copies of dna exist.
        It does not matter whether copies exist due to dna replication or cell fusion.
        */
        float recombination_during_pairing;
        /* 
        "division_during_pairing" indicates whether cell division 
        is only possible during pairing from homologous recombination.
        It prevents nonviable or less viable offspring caused by
        too many divisions without replication, or too many replications without division.
        It does not matter whether copies exist due to dna replication or cell fusion.
        
        If both division_during_pairing and recombination_during_paring are active,
        the result resembles the speculative ancestral "one-step meiosis" process 
        described by Wilkins et al. (2009).

        When homologous_recombination, division_during_pairing, stressed_autophagy, and stressed_natural_competence
        are combined, the result is a primitive form of sexual reproduction.

        Once division_during_pairing activates, the organism is stuck with whatever ploidy it had.
        If pairing fails to occur, for instance due to sufficient dissimilarity in homologous chromosomes,
        then pairing_division will prevent division until a second replication, producing a diploid organism.
        In this case, cell division as a diploid emerges naturally whenever homologous chromosomes are sufficiently dissimilar.
        */
        float division_during_pairing;

        /*
        ANOTHER LINE OF REASONING:
        A primitive unicellular organism has a repertoire of 2 actions:
        * replicate dna
        * divide cells

        The actions may initially occur in any order at any time for any reason.

        Consider the following possibilities:
        replicate * divide:               retains the ploidy
        divide * divide:                  decreases ploidy by 1, causes inviabile offspring when ploidy is 1
        replicate * replicate * divide:   increases ploidy by 1, decreases viability when ploidy is large, 
                                          otherwise improves viability if damage to dna is likely

        The repertoire may grow over time, introducting several additional actions 
        in any sequence based on evolutionarily favorable events:
        * replicate dna
        * divide cells
        * fuse cell
        * recombinate dna
        * pair homologous

        With the following additional rules:
        "fuse cells" is functionally equivalent to "replicate" with respect to its effects on ploidy.
        "pair homologous" may only occur if homologous dna exists
        If "recombinate dna" occurs in the middle of "divide cells" the result is nonviable.

        We may place any number of triggers or restrictions on these actions,
        so that one action is only allowed to occur during or immediately after another.
        Over enough time, every trigger or restriction will be attempted.
        Those that persist will improve viability. 
        Which ones are these?

                       action
                   R  D  F  Rc P 
                R                  
                D                 
        trigger F                 
                Rc                
                P                 

        Since inviability immediately results when division occurs twice, 
        the first trigger or restriction will likely prevent this scenario.
        One such restriction may limit division to situations where ploidy > 1.
        This may be accomplished by requiring that "divide" only occurs during "pairing".
        This is an especially favorable restriction, because it does not place restrictions on 
        whether multiploidy occurs due to replication or fusion.
        
        Inviability also results when recombination occurs during division,
        so we may expect another trigger or restriction against this.
        */

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(cell_wall                   );
            *output++ = encode_fraction(cytoskeleton                );
            *output++ = encode_fraction(lysin                       );
            *output++ = encode_fraction(stress_signaling            );
            *output++ = encode_fraction(phagocytosis                );
            *output++ = encode_fraction(pinocytosis                 );
            *output++ = encode_fraction(stressed_natural_competence );
            *output++ = encode_fraction(stressed_autophagy          );
            *output++ = encode_fraction(recombination_during_pairing);
            *output++ = encode_fraction(division_during_pairing     );
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            cell_wall                    = decode_fraction(*input++);
            cytoskeleton                 = decode_fraction(*input++);
            lysin                        = decode_fraction(*input++);
            stress_signaling             = decode_fraction(*input++);
            phagocytosis                 = decode_fraction(*input++);
            pinocytosis                  = decode_fraction(*input++);
            stressed_natural_competence  = decode_fraction(*input++);
            stressed_autophagy           = decode_fraction(*input++);
            recombination_during_pairing = decode_fraction(*input++);
            division_during_pairing      = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 10, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 10, 4);
        }
        static constexpr unsigned int bit_count = 10*4;
        static constexpr unsigned int attribute_count = 10;
    };
}
