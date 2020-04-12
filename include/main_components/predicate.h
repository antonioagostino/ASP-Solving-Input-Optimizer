#ifndef PREDICATE_H
#define PREDICATE_H
#include <string>

namespace aspsio {

    //  A generalization of an ASP Predicate
    class Predicate {

        protected:
            const int id;
            bool auxiliar;
            int occurrences_in_heads;
            int occurrences_in_bodies;

        public:
            Predicate(const int &_id, const bool &_auxiliar);
            int GetId(){ return id; }
            bool IsAuxiliar(){ return auxiliar; }
            int GetOccurrencesInHeads(){ return occurrences_in_heads; }
            int GetOccurrencesInBodies(){ return occurrences_in_bodies; }
            void IncrementOccurrencesInHeads(){ occurrences_in_heads++; }
            void IncrementOccurrencesInBodies(){ occurrences_in_bodies++; }
            virtual std::string DoOutput();



    };

}

#endif