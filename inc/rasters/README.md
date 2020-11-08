

the "rasters" namespace exclusively represents data structures and functions that define a mathematical category.
The category concerns the ways in which grids and rasters can be specified 
so that the attributes of an object always maintain internal consistency.

The category diagram can be seen [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgBpiBdUkANwEMAbAVxiRAGEQBfU9TXfIRQBmclVqMWbdgAIAOnLwBbeDIDK3XiAzY8BIgCZSB8fWatEIAGrzFWFXBkAxTX12CiZYackWOt5VUAGVdtfj0hZFFvajMpS1kFQMc1APtg0J0BfRQABlJcn3M2ACU0hxlOHjdsyPyAFiL4kDKk9MdEuwqQ6rD3HOR6gqa-Vq7VAHFy1Q1erIiiIcbY31Lpxym27vX1bnEYKABzeCJQADMAJwglJCMQHAgkXN7L69vqB6RiF6ubxCH7o9EAAOH5vRBkQFIADsYL++ShiAArHCkKJEQA2VGIO6fHHUBgQCBoTzA-JnRhwGDiBh0ABGMAYAAVwh5LBcsIcABY4UKvP64oH1bGCtHY9F455aflPD5A9FwLlYM68xAAWm+0t+SAxcqQKK14KResQupAiuVqo12OBJthhr+0JNtvNSpVSGtDq+JoBFvd6s1521-xNgZAMuRJuNIEJxNJ5Mp1IJ9MZLP6QhAHO5vOxTsRedjJJQxDJpApDCpNJTzNZOUznJ5fODLrxLsLRATFaTMeradqbCzjexZrxZvbeTLiarDJr6YHDZzFC4QA)

Or it can be viewed with the following tikzcd markup:

```
\begin{tikzcd}
R \times C \arrow[dddd, shift left] \arrow[loop, distance=2em, in=125, out=55] &                                     &                                                                                                   &                                              & R \times G \times S \arrow[dddd, shift left] \arrow[loop, distance=2em, in=125, out=55] \\
                                                                               & C \arrow[lu] \arrow[dd, shift left] &                                                                                                   & C \times S \arrow[ru] \arrow[dd, shift left] &                                                                                         \\
                                                                               &                                     & V \times F \arrow[lu] \arrow[ru] \arrow[loop, distance=2em, in=125, out=55] \arrow[rd] \arrow[ld] &                                              &                                                                                         \\
                                                                               & C \times L \arrow[ld] \arrow[uu]    &                                                                                                   & C \times S \times L \arrow[rd] \arrow[uu]    &                                                                                         \\
R \times C \times L \arrow[uuuu] \arrow[loop, distance=2em, in=305, out=235]   &                                     &                                                                                                   &                                              & R \times G \times L \times S \arrow[uuuu] \arrow[loop, distance=2em, in=305, out=235]  
\end{tikzcd}
```

Trivial product morphisms are omitted for brevity.
