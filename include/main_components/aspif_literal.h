#ifndef ASPIF_LITERAL_H
#define ASPIF_LITERAL_H
#include <string>

namespace aspsio {

    class AspifLiteral {

        protected:
            const int id;
            bool auxiliar;
            int occurrences_in_heads;
            int occurrences_in_bodies;

        public:
            AspifLiteral(const int &_id, const bool &_auxiliar);
            int GetId(){ return id; }
            bool IsAuxiliar(){ return auxiliar; }
            int GetOccurrencesInHeads(){ return occurrences_in_heads; }
            int GetOccurrencesInBodies(){ return occurrences_in_bodies; }
            void IncrementOccurrencesInHeads(){ occurrences_in_heads++; }
            void IncrementOccurrencesInBodies(){ occurrences_in_bodies++; }



    };

}

#endif