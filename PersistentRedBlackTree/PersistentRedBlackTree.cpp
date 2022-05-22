// Implementing Red-Black Tree in C++

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

 // Red black tree have 3 pointers pointing to each node (parent, left, right);
const int p = 3;

struct Node;
struct Mod;

struct Mod
{
  int version;
  string field;
  Node *value;
};

struct Node {
  int data;
  Node *parent;
  Node *left;
  Node *right;
  int color;
  int version;
  Mod *mods[2*p];
  unsigned int mods_length = 0;
  Node *nextVersion;
};

typedef Node *NodePtr;

bool isNodePtrValid(NodePtr node) {
  return node != nullptr && node != NULL && node->data > 0  && node->data < 38484704;
}

bool isNodeValid(Node node) {
  return node.data > 0 && node.data < 38484704;
}


void applyModToNode(NodePtr node, Mod* mod) {
  if (mod == nullptr || node == nullptr) {
    return;
  }
  string field = mod->field;
  NodePtr value = mod->value;
  if (field == "data"){
    node->data = value->data;
  } else if (field == "color"){
    node->color = value->color;
  } else if (field == "parent"){
    node->parent = value;
  } else if (field == "left"){
    node->left = value;
  } else if (field == "right"){
    node->right = value;
  }
};

NodePtr CopyNode(NodePtr node) {
  if (node == nullptr) {
    return nullptr;
  }
  NodePtr nodeCopy = new Node;
  nodeCopy->data = node->data;
  nodeCopy->color = node->color;
  nodeCopy->left = node->left;
  nodeCopy->right = node->right;
  nodeCopy->parent = node->parent;
  nodeCopy->version = node->version;
  nodeCopy->nextVersion = node->nextVersion;
  nodeCopy->mods_length = node->mods_length;  
  int l = 0;
  while(l < node->mods_length) {
    nodeCopy->mods[l] = node->mods[l];
    l++;
  }
  return nodeCopy;
}

NodePtr applyModToNewNode(NodePtr nodeParam, Mod* mod) {
  NodePtr node = CopyNode(nodeParam);  
  string field = mod->field;
  NodePtr value = mod->value;
  if (field == "data"){
    node->data = value->data;
  } else if (field == "color"){
    node->color = value->color;
  } else if (field == "parent"){
    node->parent = value;
  } else if (field == "left"){
    node->left = value;
  } else if (field == "right"){
    node->right = value;
  }
  return node;
};



NodePtr applyFieldMods (NodePtr nodeParam, string field) {
  int i = 0;
  NodePtr node = CopyNode(nodeParam);
  while (i < node->mods_length) {
    if (node->mods[i] != nullptr && node->mods[i]->field == field) {
      node = applyModToNewNode(node, node->mods[i]);
    }
    i++;
  }
  return node;
}

NodePtr getLatestNodeVersion(NodePtr nodeParam) {
  NodePtr node = CopyNode(nodeParam);
  if (node == nullptr) {
    return nullptr;
  }
  if (node->nextVersion == nullptr) {
    int i = 0;
    while(i < node->mods_length && node->mods[i] != nullptr) {
      applyModToNode(node, node->mods[i]);
      i++;
    }
    return node;
  }
  return getLatestNodeVersion(node->nextVersion);
}

void addMod(NodePtr node, string field, NodePtr value, int version) {
  if (!isNodePtrValid(node)) {
    cout << "Error in addMod: node not valid. Field: " << field << endl;
    return;
  };
  if (node->nextVersion != nullptr) {
    addMod(node->nextVersion, field, value, version);
    return;
  }
  if (node->mods_length == 2*p) {

    // Create new node
    NodePtr newNode = new Node;
    NodePtr iterNode = CopyNode(getLatestNodeVersion(node));
    newNode->data = iterNode->data;
    newNode->left = iterNode->left;
    newNode->right = iterNode->right;
    newNode->parent = iterNode->parent;
    while (iterNode->nextVersion != nullptr || isNodePtrValid(iterNode->nextVersion)) {
      iterNode = iterNode->nextVersion;
    }
    
    newNode->nextVersion = nullptr;
    newNode->version = version;
    
    // Apply mods to new node to obtain new node    
    for (int i = 0; i < iterNode->mods_length; i++){
      applyModToNode(newNode, iterNode->mods[i]);
    };

    // mods vazios
    int j = 0;
    while (j < 2*p){
      newNode->mods[j] = nullptr;
      j++;
    };

    node->nextVersion = newNode;

    // Update pointers to point to new node
    NodePtr leftNode = getLatestNodeVersion(node)->left;
    if (leftNode != nullptr && isNodePtrValid(leftNode)) {
      addMod(leftNode, "parent", newNode, version);
    }

    NodePtr rightNode = getLatestNodeVersion(node)->right;
    if (rightNode != nullptr && isNodePtrValid(rightNode)){
      addMod(rightNode, "parent", newNode, version);
    }
    NodePtr parentNode = getLatestNodeVersion(node)->parent;
    if (parentNode != nullptr && isNodePtrValid(parentNode)){
      if (parentNode->left != nullptr && isNodePtrValid(parentNode->left) &&  parentNode->left == node){
        addMod(parentNode, "left", newNode, version);
      } else if (parentNode->right != nullptr && isNodePtrValid(parentNode->right) && parentNode->right == node) {
        addMod(parentNode, "right", newNode, version);
      }
    }
    return;
  }
  Mod *mod = new Mod;
  mod->version = version;
  mod->field = field;
  mod->value = value;
  node->mods[node->mods_length] = mod;
  // node->version = version;
  node->mods_length++;
  if (mod->field == "parent") {
    node->parent = mod->value;
  }
  if (mod->field == "left") {
    node->left = mod->value;
  }
  if (mod->field == "right") {
    node->right = mod->value;
  }
}


NodePtr getNodeFieldByVersion(NodePtr node, int version, string field) {
  NodePtr nodeByVersion = node;
  int currentVersion = node->version;
  if(currentVersion <= version) {
    if (node->nextVersion == nullptr) {
      int l = 0;      
      Mod *mod;
      while (l < node->mods_length && mod->version <= version) {
        mod = node->mods[l];
        if (mod->field == field) {
          nodeByVersion = applyModToNewNode(node, mod);
        }
        l++;
      };
      return nodeByVersion;
    } else {      
      if (node->nextVersion->version < version) {
        return getNodeFieldByVersion(node->nextVersion, version, field);
      } else {
        nodeByVersion = applyFieldMods(nodeByVersion, field);
        return nodeByVersion;
      }
      return getNodeFieldByVersion(node->nextVersion, version, field);
    }
  }
  return nodeByVersion;
}

class RedBlackTree { 
  private:
    NodePtr root;
    NodePtr TNULL;

    // Sucessor
    NodePtr sucessorHelper(NodePtr node, int key) {
      NodePtr returnNode = TNULL;
      returnNode->data = numeric_limits<int>::max();
      if (node != TNULL) {
        returnNode = sucessorHelper(node->left, key);
        if (node->data > key) {
          return node;
        }
        returnNode = sucessorHelper(node->right, key);
      }
      return returnNode;
    }

    // Preorder
    void preOrderHelper(NodePtr node) {
      if (node != TNULL) {
        cout << node->data << " ";
        preOrderHelper(node->left);
        preOrderHelper(node->right);
      }
    }

    // Inorder
    void inOrderHelper(NodePtr node, int depth) {
      if (isNodePtrValid(node)) {
        int nextDepth = depth + 1;
        inOrderHelper(getLatestNodeVersion(node->left), nextDepth);
        cout << getLatestNodeVersion(node)->data << ",";
        cout << depth << ",";
        string sColor = getLatestNodeVersion(node)->color ? "R" : "N";
        cout << sColor << " ";
        inOrderHelper(getLatestNodeVersion(node->right), nextDepth);
      }
    }

    // Post order
    void postOrderHelper(NodePtr node) {
      if (node != TNULL) {
        postOrderHelper(node->left);
        postOrderHelper(node->right);
        cout << node->data << " ";
      }
    }

    NodePtr searchTreeHelper(NodePtr node, int key) {
      if (node == TNULL || key == node->data || isNodePtrValid(node) == false) {
        return node;
      }

      if (key < node->data) {
        return searchTreeHelper(node->left, key);
      }
      return searchTreeHelper(node->right, key);
    }

    // For balancing the tree after deletion
    void deleteFix(NodePtr x) {
      NodePtr s;
      NodePtr nodeAux0 = new Node;
      nodeAux0->color = 0;
      NodePtr nodeAux1 = new Node;
      nodeAux1->color = 0;
      while (x != root && getLatestNodeVersion(x)->color == 0) {
        if (x == getLatestNodeVersion(getLatestNodeVersion(x)->parent)->left) {
          s = getLatestNodeVersion(getLatestNodeVersion(x)->parent)->right;
          if (getLatestNodeVersion(s)->color == 1) {
            // s->color = 0;
            addMod(s, "color", nodeAux0, this->currentVersion);
            // x->parent->color = 1;
            addMod(getLatestNodeVersion(x)->parent, "color", nodeAux1, this->currentVersion);
            leftRotate(getLatestNodeVersion(x)->parent);
            s = getLatestNodeVersion(getLatestNodeVersion(x)->parent)->right;
          }

          if (getLatestNodeVersion(getLatestNodeVersion(s)->left)->color == 0 && getLatestNodeVersion(getLatestNodeVersion(s)->right)->color == 0) {
            // s->color = 1;
            addMod(s, "color", nodeAux1, this->currentVersion);
            x = getLatestNodeVersion(x)->parent;
          } else {
            if (getLatestNodeVersion(getLatestNodeVersion(s)->right)->color == 0) {
              // s->left->color = 0;
              addMod(getLatestNodeVersion(s)->left, "color", nodeAux0, this->currentVersion);
              // s->color = 1;
              addMod(s, "color", nodeAux1, this->currentVersion);
              rightRotate(s);
              s = getLatestNodeVersion(getLatestNodeVersion(x)->parent)->right;
            }

            // s->color = x->parent->color;
            addMod(s, "color", getLatestNodeVersion(x)->parent, this->currentVersion);
            // x->parent->color = 0;
            addMod(getLatestNodeVersion(x)->parent, "color", nodeAux0, this->currentVersion);
            // s->right->color = 0;
            addMod(getLatestNodeVersion(s)->right, "color", nodeAux0, this->currentVersion);
            leftRotate(getLatestNodeVersion(x)->parent);
            x = root;
          }
        } else {
          s = getLatestNodeVersion(getLatestNodeVersion(x)->parent)->left;
          if (s->color == 1) {
            // s->color = 0;
            addMod(s, "color", nodeAux0, this->currentVersion);
            // x->parent->color = 1;
            addMod(getLatestNodeVersion(x)->parent, "color", nodeAux1, this->currentVersion);
            rightRotate(getLatestNodeVersion(x)->parent);
            s = getLatestNodeVersion(getLatestNodeVersion(x)->parent)->left;
          }

          if (getLatestNodeVersion(getLatestNodeVersion(s)->right)->color == 0 && getLatestNodeVersion(getLatestNodeVersion(s)->right)->color == 0) {
            // s->color = 1;
            addMod(s, "color", nodeAux1, this->currentVersion);
            x = getLatestNodeVersion(x)->parent;
          } else {
            if (getLatestNodeVersion(getLatestNodeVersion(s)->left)->color == 0) {
              // s->right->color = 0;
              addMod(getLatestNodeVersion(s)->right, "color", nodeAux0, this->currentVersion);
              // s->color = 1;
              addMod(s, "color", nodeAux1, this->currentVersion);
              leftRotate(s);
              s = getLatestNodeVersion(getLatestNodeVersion(x)->parent)->left;
            }

            // s->color = x->parent->color;
            addMod(s, "color", getLatestNodeVersion(x)->parent, this->currentVersion);
            // x->parent->color = 0;
            addMod(getLatestNodeVersion(x)->parent, "color", nodeAux0, this->currentVersion);
            // s->left->color = 0;
            addMod(getLatestNodeVersion(s)->left, "color", nodeAux0, this->currentVersion);
            rightRotate(getLatestNodeVersion(x)->parent);
            x = root;
          }
        }
      }
      // x->color = 0;
      addMod(x, "color", nodeAux0, this->currentVersion);
      delete nodeAux0;
      delete nodeAux1;
    }

    void rbTransplant(NodePtr u, NodePtr v) {
      if (getLatestNodeVersion(u)->parent == nullptr) {
        root = v;
      } else if (u == getLatestNodeVersion(u->parent)->left) {
        // u->parent->left = v;
        addMod(getLatestNodeVersion(u)->parent, "left", v, this->currentVersion);
      } else {
        // u->parent->right = v;
        addMod(getLatestNodeVersion(u)->parent, "right", v, this->currentVersion);
      }
      // v->parent = u->parent;
      addMod(v, "parent", getLatestNodeVersion(u)->parent, this->currentVersion);
    }

    void deleteNodeHelper(NodePtr node, int key) {
      NodePtr z = TNULL;
      NodePtr x, y;
      while (node != TNULL) {
        if (getLatestNodeVersion(node)->data == key) {
          z = node;
        }

        if (getLatestNodeVersion(node)->data <= key) {
          node = getLatestNodeVersion(node)->right;
        } else {
          node = getLatestNodeVersion(node)->left;
        }
      }

      if (z == TNULL) {
        cout << "Key " << key << " not found in the tree" << endl;
        return;
      }

      y = z;
      int y_original_color = getLatestNodeVersion(y)->color;
      if (z->left == TNULL) {
        x = getLatestNodeVersion(z)->right;
        rbTransplant(z, getLatestNodeVersion(z)->right);
      } else if (getLatestNodeVersion(z)->right == TNULL) {
        x = z->left;
        rbTransplant(z, getLatestNodeVersion(z)->left);
      } else {
        y = minimum(getLatestNodeVersion(z)->right);
        y_original_color = getLatestNodeVersion(y)->color;
        x = getLatestNodeVersion(y)->right;
        if (getLatestNodeVersion(y)->parent == z) {
          // x->parent = y;
          addMod(x, "parent", y, this->currentVersion);
        } else {
          rbTransplant(y, getLatestNodeVersion(y)->right);
          // y->right = z->right;
          addMod(y, "right", getLatestNodeVersion(z)->right, this->currentVersion);
          // y->right->parent = y;
          addMod(getLatestNodeVersion(y)->right, "parent", y, this->currentVersion);
        }

        rbTransplant(z, y);
        // y->left = z->left;
        addMod(y, "left", getLatestNodeVersion(z)->left, this->currentVersion);
        // y->left->parent = y;
        addMod(getLatestNodeVersion(y)->left, "parent", y, this->currentVersion);
        // y->color = z->color;
        addMod(y, "color", z, this->currentVersion);
      }
      delete z;
      if (y_original_color == 0) {
        deleteFix(x);
      }      
      while (root->nextVersion != nullptr) {
        root = root->nextVersion;
      }
    }

    // For balancing the tree after insertion
    void insertFix(NodePtr k) { // Something is wrong with this fix
      NodePtr uncle;
      NodePtr nodeAux0 = new Node;
      nodeAux0->color = 0;
      NodePtr nodeAux1 = new Node;
      nodeAux1->color = 1;

      while (getLatestNodeVersion(getLatestNodeVersion(k)->parent)->color == 1 && isNodePtrValid(getLatestNodeVersion(k)->parent) && isNodePtrValid(getLatestNodeVersion(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent)) ) {
        if (getLatestNodeVersion(k)->parent == getLatestNodeVersion(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent)->right) {
          uncle = getLatestNodeVersion(getLatestNodeVersion(k)->parent->parent)->left;
          if (getLatestNodeVersion(uncle)->color == 1) {            
            // u->color = 0;
            addMod(uncle, "color", nodeAux0, this->currentVersion);
            // k->parent->color = 0;
            addMod(getLatestNodeVersion(k)->parent, "color", nodeAux0, this->currentVersion);
            // k->parent->parent->color = 1;
            addMod(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent, "color", nodeAux1, this->currentVersion);
            k = getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent;            
          } else {
            if (getLatestNodeVersion(k) == getLatestNodeVersion(getLatestNodeVersion(k)->parent)->left) {
              k = getLatestNodeVersion(k)->parent;
              rightRotate(k);
            }
            // k->parent->color = 0;
            addMod(getLatestNodeVersion(k)->parent, "color", nodeAux0, this->currentVersion);

            // k->parent->parent->color = 1;
            addMod(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent, "color", nodeAux1, this->currentVersion);
            leftRotate(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent);            
          }
        } else {        
          uncle = getLatestNodeVersion(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent)->right;

          if (getLatestNodeVersion(uncle)->color == 1) {
            // u->color = 0;
            addMod(uncle, "color", nodeAux0, this->currentVersion);
            // k->parent->color = 0;
            addMod(getLatestNodeVersion(k)->parent, "color", nodeAux0, this->currentVersion);
            // k->parent->parent->color = 1;
            addMod(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent, "color", nodeAux1, this->currentVersion);
            k = getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent;            
          } else {
            if (k == getLatestNodeVersion(getLatestNodeVersion(k)->parent)->right) {
              k = getLatestNodeVersion(k)->parent;
              leftRotate(getLatestNodeVersion(k));
            }
            // k->parent->color = 0;
            addMod(getLatestNodeVersion(k)->parent, "color", nodeAux0, this->currentVersion);
            // k->parent->parent->color = 1;
            addMod(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent, "color", nodeAux1, this->currentVersion);
            rightRotate(getLatestNodeVersion(getLatestNodeVersion(k)->parent)->parent);
          }
        }
        if (k == root || !isNodePtrValid(k) || !isNodePtrValid(getLatestNodeVersion(k)->parent)) {
          break;
        }
      }   

      // root->color = 0;
      addMod(root, "color", nodeAux0, this->currentVersion);
      while (root->nextVersion != nullptr) {
        this->root = this->root->nextVersion;
      }
      delete nodeAux0;
      delete nodeAux1;
    }

    void printHelper(NodePtr root, string indent, bool last) {
      if (isNodePtrValid(root) && root != TNULL) {
        cout << indent;
        if (last) {
          cout << "R----";
          indent += "   ";
        } else {
          cout << "L----";
          indent += "|  ";
        }
        string sColor = root->color ? "RED" : "BLACK";
        cout << root->data << "(" << sColor << ")" << endl;
        printHelper(root->left, indent, false);
        printHelper(root->right, indent, true);
      }
    }

  public:
    NodePtr roots[100];
    int currentVersion;
    RedBlackTree *previousTree;
    
    RedBlackTree() {
      TNULL = new Node;
      TNULL->color = 0;
      TNULL->version = 0;
      TNULL->left = nullptr;
      TNULL->right = nullptr;
      root = TNULL;
      root->nextVersion = nullptr;
      int j = 0;
      while (j < 2*p){
        root->mods[j] = nullptr;
        j++;
      };
      
      roots[0] = root;
      this->currentVersion = 0;
      previousTree = nullptr;
    }

    void initializeNULLNode(NodePtr node, NodePtr parent) {
      node->data = 0;
      node->parent = parent;
      node->left = nullptr;
      node->right = nullptr;
      node->color = 0;
    }

    void preorder() {
      preOrderHelper(this->root);
    }

    void inorderByVersion(int version) {      
      NodePtr node = getLatestNodeVersion(this->roots[version]);
      this->inorder(node);
    }

    void inorder(NodePtr node) {
      inOrderHelper(node, 0);
      cout << endl;
    }

    void postorder() {
      postOrderHelper(this->root);
    }

    NodePtr searchTree(int k) {
      return searchTreeHelper(this->root, k);
    }

    NodePtr minimum(NodePtr node) {
      if (isNodePtrValid(node)) {
        while (node->left != TNULL && isNodePtrValid(node->left)) {
          node = node->left;
        }
      }
      return node;
    }

    NodePtr maximum(NodePtr node) {
      while (node->right != TNULL) {
        node = node->right;
      }
      return node;
    }

    NodePtr successor(int key, int version) {
      // RedBlackTree* tree = getTreeByVersion(version);
      NodePtr x = this->searchTree(key);

      if (x == TNULL) {
        return sucessorHelper(this->getRoot(), key);
      }
      if (x->right != TNULL) {
        return minimum(x->right);
      }

      NodePtr y = x->parent;
      while (y != TNULL && x == y->right) {
        x = y;
        y = y->parent;
      }
      return y;
    }

    NodePtr predecessor(int key) {
      NodePtr x = searchTree(key);
      if (x->left != TNULL) {
        return maximum(x->left);
      }

      NodePtr y = x->parent;
      while (y != TNULL && x == y->left) {
        x = y;
        y = y->parent;
      }

      return y;
    }

    void leftRotate(NodePtr x) {
      NodePtr y = getLatestNodeVersion(x)->right;
      // x->right = y->left;
      addMod(x, "right", getLatestNodeVersion(y)->left, this->currentVersion);
      if (getLatestNodeVersion(y)->left != TNULL) {
        // y->left->parent = x;
        addMod(getLatestNodeVersion(y)->left, "parent", x, this->currentVersion);
      }
      // y->parent = x->parent;
      addMod(y, "parent", getLatestNodeVersion(x)->parent, this->currentVersion);
      if (getLatestNodeVersion(x)->parent == nullptr) {
        this->root = y;
      } else if (x == getLatestNodeVersion(getLatestNodeVersion(x)->parent)->left) {
        // x->parent->left = y;
        addMod(getLatestNodeVersion(x)->parent, "left", y, this->currentVersion);
      } else {
        // x->parent->right = y;
        addMod(getLatestNodeVersion(x)->parent, "right", y, this->currentVersion);
      }
      // y->left = x;
      addMod(y, "left", x, this->currentVersion);
      // x->parent = y;
      addMod(x, "parent", y, this->currentVersion);
    }

    void rightRotate(NodePtr x) {
      NodePtr y = getLatestNodeVersion(x)->left;
      // x->left = y->right;
      addMod(getLatestNodeVersion(x)->left, "right", getLatestNodeVersion(y)->right, this->currentVersion);
      if (getLatestNodeVersion(y)->right != TNULL) {
        // y->right->parent = x;
        addMod(getLatestNodeVersion(y)->right, "parent", x, this->currentVersion);
      }
      // y->parent = x->parent;
      addMod(y, "parent", getLatestNodeVersion(x)->parent, this->currentVersion);
      if (getLatestNodeVersion(x)->parent == nullptr) {
        this->root = y;
      } else if (x == getLatestNodeVersion(getLatestNodeVersion(x)->parent)->right) {
        // x->parent->right = y;
        addMod(getLatestNodeVersion(x)->parent, "right", y, this->currentVersion);
      } else {
        // x->parent->left = y;
        addMod(getLatestNodeVersion(x)->parent, "left", y, this->currentVersion);
      }
      // y->right = x;
      addMod(y, "right", x, this->currentVersion);
      // x->parent = y;
      addMod(x, "parent", y, this->currentVersion);
    }

    // Inserting a node
    void insert(int key) {    
      // this->inorderByVersion(this->currentVersion);  
      this->currentVersion++;

      NodePtr node = new Node;
      node->parent = nullptr;
      node->data = key;
      node->left = TNULL;
      node->right = TNULL;
      node->color = 1;
      node->version = this->currentVersion;
      node->nextVersion = nullptr;
      node->mods_length = 0;
      int j = 0;
      while (j < 2*p){
        node->mods[j] = nullptr;
        j++;
      };
      
      NodePtr y = nullptr;
      NodePtr x = this->root;

      while (isNodePtrValid(x) && x != TNULL) {
        y = x;
        if (node->data < x->data) {
          x = x->left;
        } else {
          x = x->right;
        }
      }
      // node->parent = y;
      addMod(node, "parent", y, this->currentVersion);
      if (y == nullptr) {
        node->mods_length = root->mods_length;
        root = node;
      } else if (node->data < y->data) {
        // y->left = node;
        addMod(y, "left", getLatestNodeVersion(node), this->currentVersion);
      } else {
        // y->right = node;
        addMod(y, "right", getLatestNodeVersion(node), this->currentVersion);
      }


      if (getLatestNodeVersion(node)->parent == nullptr) {
        // node->color = 0;
        NodePtr auxNode = new Node;
        auxNode->color = 0;
        addMod(node, "color", auxNode, this->currentVersion);
        this->roots[this->currentVersion] = this->root;
        this->inorderByVersion(this->currentVersion);
        return;
      }


      if (getLatestNodeVersion(node->parent)->parent == nullptr) {
        this->roots[this->currentVersion] = this->root;
        this->inorderByVersion(this->currentVersion);
        return;
      }
      insertFix(node);
      // Save current version;      
      this->roots[this->currentVersion] = this->root;
      this->inorderByVersion(this->currentVersion);
    }

    NodePtr getRoot() {
      return this->root;
    }

    void deleteNode(int data) {      
      this->currentVersion++;
      deleteNodeHelper(this->root, data);
      this->roots[this->currentVersion] = this->root;
      this->inorderByVersion(this->currentVersion);
    }

    void printTree() {
      cout << "Printing tree version " << this->currentVersion << endl;
      if (root) {
        printHelper(this->root, "", true);
      }
    }
};

int main() {
  RedBlackTree bst;

  std::string fileContent;
  std::ifstream entryFile("treeExample.txt");
  
  if (entryFile.is_open()) {
    std::string line;
    while (entryFile.good()) {
      std::getline(entryFile, line);
      std::string command = line.substr(0, line.find(" "));
      if (command == "INC") {
        std::string value = line.substr(line.find(" "), line.find("\n"));
        std::cout << "INC" << value << std::endl;
        bst.insert(stoi(value));
      } else if (command == "REM") {
        std::string value = line.substr(line.find(" "), line.find("\n"));
        std::cout << "REM" << value << std::endl;
        bst.deleteNode(stoi(value));
      } else if (command == "SUC") {
        std::string data = line.substr(line.find(" "), line.find("\n"));
        std::string valueToFindSucessor = data.substr(0, line.find(" "));
        std::string version = data.substr(line.find(" "), line.find("\n"));
        NodePtr node = bst.successor(stoi(valueToFindSucessor), stoi(version));
        std::cout << "SUC" << valueToFindSucessor << version << std::endl;
        std::cout << node->data << std::endl;
      } else if (command == "IMP") {
        std::string version = line.substr(line.find(" "), line.find("\n"));
        std::cout << "IMP" << version  << std::endl;
        bst.inorderByVersion(stoi(version));
      }
    }
  } else {
    std::cout << "Couldn't open file\n";
  }

  return 0;  
}