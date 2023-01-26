#include <string>
#include <sstream>

#include "Card.h"

// You may want to write the following three functions, but they are not required

// std::istream& operator>>(std::istream& in, Suit& suit) {
//    
// }

// Rank to_rank(const std::string& string) {
//    
// }

// std::istream& operator>>(std::istream& in, Card& card) {
//    
// }

List<Card> buildDeck(std::istream& file) {
    List<Card> l;
    std::string hold = "";
    while(getline(file,hold)) {
        std::stringstream ss = std::stringstream(hold);
        Card placeholder;
        std::string hold_suit = "";
        std::getline(ss, hold_suit, ' ');
        std::string hold_rank = "";
        std::getline(ss, hold_rank, ' ');
        int i_rank = 0;
        if (hold_rank == "ace") {
            i_rank = 1;
        }
        else if (hold_rank =="jack") {
            i_rank = 11;
        }
        else if (hold_rank == "queen") {
            i_rank = 12;
        }
        else if (hold_rank == "king") {
            i_rank = 13;
        }
        else {
            i_rank = std::stoi(hold_rank);
        }
        if (hold_suit == "spades") {
            placeholder.suit = Suit::SPADES;
        }
        if (hold_suit == "clubs") {
            placeholder.suit = Suit::CLUBS;
        }
        if (hold_suit == "diamonds") {
            placeholder.suit = Suit::DIAMONDS;
        }
        if (hold_suit == "hearts") {
            placeholder.suit = Suit::HEARTS;
        }
        placeholder.rank = i_rank;
        l.push_back(placeholder);
    }
    return l;
}

List<Card> shuffle(const List<Card>& deck) {
    List<Card> ret;
    for (const Card& card : deck) {
        if (rand221() % 2 == 0) { //coin flip condition results in heads
            ret.push_front(card);
        }
        else { // results in tails
            ret.push_back(card);
        }
    }
    // typename List<Card>::iterator hold = typename List<Card>::iterator(&deck.head);
    // for (int i = 0; i < deck.size(); i++) {
    //     hold++;
    //     if (rand221() % 2 == 0) { //coin flip condition results in heads
    //         ret.push_front(hold);
    //     }
    //     else { // results in tails
    //         ret.push_back(hold);
    //     }
    // }
    return ret;
}
