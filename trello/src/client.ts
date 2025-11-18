// client.ts
interface DragData {
  cardId: string;
  fromListId: string;
}

class TrelloApp {
  private draggedCard: HTMLElement | null = null;
  private currentListId: string = '';

  constructor() {
    this.init();
  }

  private init(): void {
    this.setupDragAndDrop();
    this.setupCardModal();
    this.setupListModal();
    this.setupDeleteButtons();
  }

  private setupDragAndDrop(): void {
    // Handle card drag start
    document.addEventListener('dragstart', (e: DragEvent) => {
      const target = e.target as HTMLElement;
      if (target.classList.contains('card')) {
        this.draggedCard = target;
        target.classList.add('opacity-50');
        
        const cardId = target.dataset.cardId!;
        const listId = target.dataset.listId!;
        
        e.dataTransfer!.effectAllowed = 'move';
        e.dataTransfer!.setData('text/plain', JSON.stringify({ cardId, fromListId: listId }));
      }
    });

    // Handle drag end
    document.addEventListener('dragend', (e: DragEvent) => {
      const target = e.target as HTMLElement;
      if (target.classList.contains('card')) {
        target.classList.remove('opacity-50');
        this.draggedCard = null;
      }
    });

    // Handle drag over (must prevent default to allow drop)
    document.addEventListener('dragover', (e: DragEvent) => {
      e.preventDefault();
      const target = e.target as HTMLElement;
      const cardsContainer = target.closest('.cards') as HTMLElement;
      
      if (cardsContainer) {
        e.dataTransfer!.dropEffect = 'move';
        cardsContainer.classList.add('bg-blue-50');
      }
    });

    // Handle drag leave
    document.addEventListener('dragleave', (e: DragEvent) => {
      const target = e.target as HTMLElement;
      const cardsContainer = target.closest('.cards') as HTMLElement;
      
      if (cardsContainer && !cardsContainer.contains(e.relatedTarget as Node)) {
        cardsContainer.classList.remove('bg-blue-50');
      }
    });

    // Handle drop
    document.addEventListener('drop', async (e: DragEvent) => {
      e.preventDefault();
      const target = e.target as HTMLElement;
      const cardsContainer = target.closest('.cards') as HTMLElement;
      
      if (cardsContainer && this.draggedCard) {
        cardsContainer.classList.remove('bg-blue-50');
        
        const data: DragData = JSON.parse(e.dataTransfer!.getData('text/plain'));
        const toListId = cardsContainer.dataset.listId!;
        
        if (data.fromListId !== toListId) {
          try {
            await this.moveCard(data.cardId, data.fromListId, toListId);
            cardsContainer.appendChild(this.draggedCard);
            this.draggedCard.dataset.listId = toListId;
          } catch (error) {
            console.error('Error moving card:', error);
            alert('Failed to move card');
          }
        } else {
          cardsContainer.appendChild(this.draggedCard);
        }
      }
    });
  }

  private setupCardModal(): void {
    const modal = document.getElementById('card-modal')!;
    const saveBtn = document.getElementById('save-card')!;
    const cancelBtn = document.getElementById('cancel-card')!;
    const titleInput = document.getElementById('card-title') as HTMLInputElement;
    const descInput = document.getElementById('card-description') as HTMLTextAreaElement;

    // Add card buttons
    document.addEventListener('click', (e: MouseEvent) => {
      const target = e.target as HTMLElement;
      if (target.classList.contains('add-card-btn') || target.closest('.add-card-btn')) {
        const btn = target.classList.contains('add-card-btn') ? target : target.closest('.add-card-btn')!;
        this.currentListId = btn.getAttribute('data-list-id')!;
        modal.classList.remove('hidden');
        titleInput.value = '';
        descInput.value = '';
        titleInput.focus();
      }
    });

    // Save card
    saveBtn.addEventListener('click', async () => {
      const title = titleInput.value.trim();
      const description = descInput.value.trim();

      if (title) {
        try {
          await this.addCard(title, description, this.currentListId);
          window.location.reload(); // Refresh to show new card
        } catch (error) {
          console.error('Error adding card:', error);
          alert('Failed to add card');
        }
      }
    });

    // Cancel
    cancelBtn.addEventListener('click', () => {
      modal.classList.add('hidden');
    });

    // Close on ESC key
    document.addEventListener('keydown', (e: KeyboardEvent) => {
      if (e.key === 'Escape' && !modal.classList.contains('hidden')) {
        modal.classList.add('hidden');
      }
    });
  }

  private setupListModal(): void {
    const modal = document.getElementById('list-modal')!;
    const addListBtn = document.getElementById('add-list-btn')!;
    const saveBtn = document.getElementById('save-list')!;
    const cancelBtn = document.getElementById('cancel-list')!;
    const titleInput = document.getElementById('list-title') as HTMLInputElement;

    addListBtn.addEventListener('click', () => {
      modal.classList.remove('hidden');
      titleInput.value = '';
      titleInput.focus();
    });

    saveBtn.addEventListener('click', async () => {
      const title = titleInput.value.trim();
      if (title) {
        try {
          await this.addList(title);
          window.location.reload(); // Refresh to show new list
        } catch (error) {
          console.error('Error adding list:', error);
          alert('Failed to add list');
        }
      }
    });

    cancelBtn.addEventListener('click', () => {
      modal.classList.add('hidden');
    });

    document.addEventListener('keydown', (e: KeyboardEvent) => {
      if (e.key === 'Escape' && !modal.classList.contains('hidden')) {
        modal.classList.add('hidden');
      }
    });
  }

  private setupDeleteButtons(): void {
    // Delete card
    document.addEventListener('click', async (e: MouseEvent) => {
      const target = e.target as HTMLElement;
      const deleteBtn = target.closest('.delete-card') as HTMLElement;
      
      if (deleteBtn) {
        e.stopPropagation();
        const cardId = deleteBtn.dataset.cardId!;
        const listId = deleteBtn.dataset.listId!;
        
        if (confirm('Delete this card?')) {
          try {
            await this.deleteCard(cardId, listId);
            const card = document.querySelector(`[data-card-id="${cardId}"]`);
            card?.remove();
          } catch (error) {
            console.error('Error deleting card:', error);
            alert('Failed to delete card');
          }
        }
      }
    });

    // Delete list
    document.addEventListener('click', async (e: MouseEvent) => {
      const target = e.target as HTMLElement;
      const deleteBtn = target.closest('.delete-list') as HTMLElement;
      
      if (deleteBtn) {
        e.stopPropagation();
        const listId = deleteBtn.dataset.listId!;
        
        if (confirm('Delete this list and all its cards?')) {
          try {
            await this.deleteList(listId);
            const list = document.querySelector(`[data-list-id="${listId}"].list`);
            list?.remove();
          } catch (error) {
            console.error('Error deleting list:', error);
            alert('Failed to delete list');
          }
        }
      }
    });
  }

  // API calls
  private async addCard(title: string, description: string, listId: string): Promise<void> {
    const response = await fetch('/api/cards', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ title, description, listId })
    });

    if (!response.ok) {
      throw new Error('Failed to add card');
    }
  }

  private async deleteCard(cardId: string, listId: string): Promise<void> {
    const response = await fetch(`/api/cards/${listId}/${cardId}`, {
      method: 'DELETE'
    });

    if (!response.ok) {
      throw new Error('Failed to delete card');
    }
  }

  private async addList(title: string): Promise<void> {
    const response = await fetch('/api/lists', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ title })
    });

    if (!response.ok) {
      throw new Error('Failed to add list');
    }
  }

  private async deleteList(listId: string): Promise<void> {
    const response = await fetch(`/api/lists/${listId}`, {
      method: 'DELETE'
    });

    if (!response.ok) {
      throw new Error('Failed to delete list');
    }
  }

  private async moveCard(cardId: string, fromListId: string, toListId: string): Promise<void> {
    const response = await fetch('/api/cards/move', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ cardId, fromListId, toListId })
    });

    if (!response.ok) {
      throw new Error('Failed to move card');
    }
  }
}

// Initialize app when DOM is ready
if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', () => new TrelloApp());
} else {
  new TrelloApp();
}