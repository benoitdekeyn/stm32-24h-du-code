// LED state tracking
let ledState = false;

// DOM elements
const toggleBtn = document.getElementById('toggleBtn');
const statusText = document.getElementById('statusText');

// API endpoint
const API_URL = '/api/toggle';

/**
 * Toggle LED via API
 */
async function toggleLED() {
    try {
        // Disable button during request
        toggleBtn.disabled = true;
        
        // Send POST request to toggle endpoint
        const response = await fetch(API_URL, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            }
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        // Parse JSON response
        const data = await response.json();
        
        // Update UI based on response
        if (data.status === 'success') {
            ledState = !ledState;
            updateStatusDisplay(data.message);
        } else {
            console.error('Toggle failed:', data);
            updateStatusDisplay('Error');
        }
        
    } catch (error) {
        console.error('Error toggling LED:', error);
        updateStatusDisplay('Connection Error');
    } finally {
        // Re-enable button
        toggleBtn.disabled = false;
    }
}

/**
 * Update status display
 * @param {string} message - Status message from server
 */
function updateStatusDisplay(message) {
    statusText.textContent = message;
    
    // Remove existing state classes
    statusText.classList.remove('on', 'off');
    
    // Add appropriate class based on message
    if (message.includes('ON')) {
        statusText.classList.add('on');
    } else if (message.includes('OFF')) {
        statusText.classList.add('off');
    }
}

// Event listener
toggleBtn.addEventListener('click', toggleLED);

// Initialize status
updateStatusDisplay('Ready');
