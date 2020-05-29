#ifndef ASPIF_LITERAL_H
#define ASPIF_LITERAL_H
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

namespace aspsio {

    class AspifLiteral {

        protected:
            const int id;
            bool auxiliar;
            int occurrences_in_heads;
            int occurrences_in_bodies;
            int rewriting_type_id;
            std::vector<std::shared_ptr<AspifLiteral>> depends_on;

        public:
            AspifLiteral(const int &_id, const bool &_auxiliar);
            int GetId(){ return id; }
            bool IsAuxiliar(){ return auxiliar; }
            int GetOccurrencesInHeads(){ return occurrences_in_heads; }
            int GetOccurrencesInBodies(){ return occurrences_in_bodies; }
            void IncrementOccurrencesInHeads(){ occurrences_in_heads++; }
            void IncrementOccurrencesInBodies(){ occurrences_in_bodies++; }
            void AddLiteralDependency(std::shared_ptr<AspifLiteral> literal);
            std::vector<std::shared_ptr<AspifLiteral>>& GetLiteralsDependecies(){ return depends_on; }
            void SetRewritingType(const int &_id){ rewriting_type_id = _id; }
            int GetRewritingType(){ return rewriting_type_id; }



    };

}

#endif