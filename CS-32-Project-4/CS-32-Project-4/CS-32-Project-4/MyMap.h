// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef mymap_h
#define mymap_h

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(){
        m_size = 0;
        m_rootNode = nullptr;
    }
    
    ~MyMap(){
        clear();
    }
    
    void clear(){
        deleteTree(m_rootNode);
        m_rootNode = nullptr;
    }
    
    int size() const {return m_size;};
    
    void associate(const KeyType& key, const ValueType& value){
        m_size++;
        
        //Empty Tree
        if(m_rootNode == nullptr){
            m_rootNode = new Node(key, value);
            return;
        }
        
        Node* cur = m_rootNode;
        
        //Finds suitable place to insert into tree
        for(;;){
            if(cur->m_first == key){
                cur->m_second = value;
                m_size--; //Duplicate key, no need to increase size
                return;
            }
            
            if(key < cur->m_first){
                if(cur->m_left != nullptr)
                    cur = cur->m_left;
                else{
                    cur->m_left = new Node(key, value);
                    return;
                }
                    
            } else if(cur->m_first < key){
                if(cur->m_right != nullptr)
                    cur = cur->m_right;
                else{
                    cur->m_right = new Node(key, value);
                    return;
                }
            }
        }
    }

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const{
        Node* cur = m_rootNode;
        
        while(cur != nullptr){
            if(cur->m_first == key)
                return &cur->m_second;
            
            if(key < cur->m_first)
                cur = cur->m_left;
            else
                cur = cur->m_right;
        }
        
        return nullptr; //Not found!
    }

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    struct Node {
        Node(KeyType first, ValueType second) : m_first(first), m_second(second){
            m_left = m_right = nullptr;
        }
        
        Node(KeyType first, ValueType second, Node* leftChild, Node* rightChild) : m_first(first), m_second(second), m_left(leftChild), m_right(rightChild){
        }
        
        KeyType m_first;
        ValueType m_second;
        
        Node* m_left;
        Node* m_right;
    };
    
    Node* m_rootNode;
    int m_size;
    
    void deleteTree(Node* rootNode){
        if(rootNode == nullptr) return;
        deleteTree(rootNode->m_left);
        deleteTree(rootNode->m_right);
        
        delete rootNode;
    }
};

#endif // mymap_h
