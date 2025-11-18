"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
// server.ts
const express_1 = __importDefault(require("express"));
const app = (0, express_1.default)();
const PORT = 3000;
// Middleware
app.use(express_1.default.json());
app.use(express_1.default.static('public'));
app.use('/dist', express_1.default.static('dist'));
// In-memory database
let lists = [
    {
        id: '1',
        title: 'To Do',
        cards: [
            { id: '1', title: 'Task 1', description: 'Description 1', listId: '1' },
            { id: '2', title: 'Task 2', description: 'Description 2', listId: '1' }
        ]
    },
    {
        id: '2',
        title: 'In Progress',
        cards: [
            { id: '3', title: 'Task 3', description: 'Description 3', listId: '2' }
        ]
    },
    {
        id: '3',
        title: 'Done',
        cards: []
    }
];
// Helper function to generate HTML
function generateHTML(lists) {
    const listsHTML = lists.map(list => `
    <div class="list bg-gray-100 rounded-lg p-4 min-w-[300px] max-w-[300px]" data-list-id="${list.id}">
      <div class="flex items-center justify-between mb-4">
        <h3 class="font-semibold text-gray-800">${list.title}</h3>
        <button class="delete-list text-gray-500 hover:text-red-500" data-list-id="${list.id}">
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
          </svg>
        </button>
      </div>
      <div class="cards space-y-2 min-h-[100px]" data-list-id="${list.id}">
        ${list.cards.map(card => `
          <div class="card bg-white rounded shadow p-3 cursor-move hover:shadow-md transition-shadow" 
               draggable="true" 
               data-card-id="${card.id}"
               data-list-id="${list.id}">
            <div class="flex items-start justify-between">
              <div class="flex-1">
                <h4 class="font-medium text-gray-900 mb-1">${card.title}</h4>
                <p class="text-sm text-gray-600">${card.description}</p>
              </div>
              <button class="delete-card text-gray-400 hover:text-red-500 ml-2" 
                      data-card-id="${card.id}"
                      data-list-id="${list.id}">
                <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"></path>
                </svg>
              </button>
            </div>
          </div>
        `).join('')}
      </div>
      <button class="add-card-btn w-full mt-3 text-left text-gray-600 hover:text-gray-800 px-2 py-1 rounded hover:bg-gray-200" data-list-id="${list.id}">
        + Add a card
      </button>
    </div>
  `).join('');
    return `
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Trello Clone</title>
  <script src="https://cdn.tailwindcss.com"></script>
</head>
<body class="bg-gradient-to-br from-blue-500 to-purple-600 min-h-screen">
  <div class="container mx-auto p-6">
    <header class="mb-6">
      <h1 class="text-3xl font-bold text-white mb-2">Trello Clone</h1>
      <p class="text-white/80">Organize your tasks with drag and drop</p>
    </header>
    
    <div class="board flex gap-4 overflow-x-auto pb-4">
      ${listsHTML}
      <button id="add-list-btn" class="add-list min-w-[300px] max-w-[300px] bg-white/20 hover:bg-white/30 rounded-lg p-4 text-white font-semibold h-fit">
        + Add a list
      </button>
    </div>
  </div>

  <!-- Add Card Modal -->
  <div id="card-modal" class="hidden fixed inset-0 bg-black/50 flex items-center justify-center z-50">
    <div class="bg-white rounded-lg p-6 w-96">
      <h3 class="text-xl font-bold mb-4">Add Card</h3>
      <input type="text" id="card-title" placeholder="Card title" class="w-full border rounded px-3 py-2 mb-3">
      <textarea id="card-description" placeholder="Description" class="w-full border rounded px-3 py-2 mb-4 h-24"></textarea>
      <div class="flex gap-2">
        <button id="save-card" class="bg-blue-500 text-white px-4 py-2 rounded hover:bg-blue-600">Save</button>
        <button id="cancel-card" class="bg-gray-300 text-gray-800 px-4 py-2 rounded hover:bg-gray-400">Cancel</button>
      </div>
    </div>
  </div>

  <!-- Add List Modal -->
  <div id="list-modal" class="hidden fixed inset-0 bg-black/50 flex items-center justify-center z-50">
    <div class="bg-white rounded-lg p-6 w-96">
      <h3 class="text-xl font-bold mb-4">Add List</h3>
      <input type="text" id="list-title" placeholder="List title" class="w-full border rounded px-3 py-2 mb-4">
      <div class="flex gap-2">
        <button id="save-list" class="bg-blue-500 text-white px-4 py-2 rounded hover:bg-blue-600">Save</button>
        <button id="cancel-list" class="bg-gray-300 text-gray-800 px-4 py-2 rounded hover:bg-gray-400">Cancel</button>
      </div>
    </div>
  </div>

  <script src="/dist/client.js"></script>
</body>
</html>
  `;
}
// Routes
app.get('/', (req, res) => {
    res.send(generateHTML(lists));
});
// API Routes
app.get('/api/lists', (req, res) => {
    res.json(lists);
});
app.post('/api/lists', (req, res) => {
    const newList = {
        id: Date.now().toString(),
        title: req.body.title,
        cards: []
    };
    lists.push(newList);
    res.json(newList);
});
app.delete('/api/lists/:id', (req, res) => {
    lists = lists.filter(list => list.id !== req.params.id);
    res.json({ success: true });
});
app.post('/api/cards', (req, res) => {
    const { title, description, listId } = req.body;
    const list = lists.find(l => l.id === listId);
    if (list) {
        const newCard = {
            id: Date.now().toString(),
            title,
            description,
            listId
        };
        list.cards.push(newCard);
        res.json(newCard);
    }
    else {
        res.status(404).json({ error: 'List not found' });
    }
});
app.delete('/api/cards/:listId/:cardId', (req, res) => {
    const { listId, cardId } = req.params;
    const list = lists.find(l => l.id === listId);
    if (list) {
        list.cards = list.cards.filter(card => card.id !== cardId);
        res.json({ success: true });
    }
    else {
        res.status(404).json({ error: 'List not found' });
    }
});
app.post('/api/cards/move', (req, res) => {
    const { cardId, fromListId, toListId } = req.body;
    const fromList = lists.find(l => l.id === fromListId);
    const toList = lists.find(l => l.id === toListId);
    if (fromList && toList) {
        const cardIndex = fromList.cards.findIndex(c => c.id === cardId);
        if (cardIndex !== -1) {
            const [card] = fromList.cards.splice(cardIndex, 1);
            card.listId = toListId;
            toList.cards.push(card);
            res.json({ success: true });
        }
        else {
            res.status(404).json({ error: 'Card not found' });
        }
    }
    else {
        res.status(404).json({ error: 'List not found' });
    }
});
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});
