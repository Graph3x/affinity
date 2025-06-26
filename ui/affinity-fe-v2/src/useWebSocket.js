import { useEffect, useState } from "react";

const addWithLimit = (prev, newStuff) => {
  
  const newData = [...prev, newStuff]
  if (newData.length > 2) {
    newData.shift();
  }
  return newData;
}


export function useWebSocket(url) {
  const [messages, setMessages] = useState([]);

  useEffect(() => {
    const ws = new WebSocket(url);

    ws.onmessage = (event) => {
      setMessages((prev) => addWithLimit(prev, event.data));
    };

    ws.onerror = (error) => {
      console.error("WebSocket error:", error);
    };

    return () => {
      ws.close();
    };
  }, [url]);

  return messages;
}
